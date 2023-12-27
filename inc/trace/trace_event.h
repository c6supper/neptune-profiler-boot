#ifndef TRACE_EVENT_H_
#define TRACE_EVENT_H_

#include <cstdint>
#include <iostream>
#include <memory>

#include "json.hpp"
#include "logger.h"
#include "trace_type.h"

namespace coding_nerd::boot_perf {

template <typename T>
struct TraceEvent {
  std::shared_ptr<T> event_;
  TraceEvent() = delete;
  TraceEvent(TraceEvent& other) = delete;
  TraceEvent& operator=(TraceEvent& other) = delete;
  TraceEvent(TraceEvent&& other) = delete;
  virtual ~TraceEvent() = default;
  explicit TraceEvent(std::shared_ptr<T> event) : event_(event){};  // NOLINT

  //  public:
  //   TraceEvent() = delete;
  //   TraceEvent(TraceEvent& other) = delete;
  //   TraceEvent& operator=(TraceEvent& other) = delete;
  //   TraceEvent(TraceEvent&& other) = default;
  //   explicit TraceEvent(std::shared_ptr<T>&& event) :
  //   event_(std::move(event)) {
  //     toExt(_NTO_TRACE_GETEVENT_C(event->header),
  //           _NTO_TRACE_GETEVENT(event->header), &class_, &type_);
  //     cpu_ = _NTO_TRACE_GETCPU(event->header);
  //     timestamp_ = _NTO_TRACE_GETCPU(event->data[0]);
  //   };

  //  private:
  //   const std::shared_ptr<T> event_;
  //   class_type class_;
  //   class_type type_;
  //   cpu_type cpu_;
  //   uint32_t timestamp_;
  // NOLINTBEGIN
  static void ToExt(const uint32_t int_class, uint32_t int_event,
                    uint32_t& ext_class, uint32_t& ext_event) {
    int event_64 = 0;

    switch (int_class) {
      case _TRACE_COMM_C:
        ext_class = _NTO_TRACE_COMM;
        ext_event = int_event;
        break;

      case _TRACE_CONTROL_C:
        ext_class = _NTO_TRACE_CONTROL;
        ext_event = int_event;
        break;

      case _TRACE_INT_C:
        ext_event = -1;
        switch (int_event) {
          case _TRACE_INT_ENTRY:
            ext_class = _NTO_TRACE_INTENTER;
            break;

          case _TRACE_INT_EXIT:
            ext_class = _NTO_TRACE_INTEXIT;
            break;

          case _TRACE_INT_HANDLER_ENTRY:
            ext_class = _NTO_TRACE_INT_HANDLER_ENTER;
            break;

          case _TRACE_INT_HANDLER_EXIT:
            ext_class = _NTO_TRACE_INT_HANDLER_EXIT;
            break;

          default:
            Warning() << "Unknown Interrupt event: " << int_event;
        }
        break;

      case _TRACE_KER_CALL_C:

        /* Remove _NTO_TRACE_KERCALL64 if it's set. */
        if (int_event & _NTO_TRACE_KERCALL64) {
          event_64 = 1;
          int_event = int_event & ~_NTO_TRACE_KERCALL64;
        }

        /* Determine the class and event. */
        if (int_event < _TRACE_MAX_KER_CALL_NUM) {
          ext_class = _NTO_TRACE_KERCALLENTER;
          ext_event = int_event;
        } else if (int_event < 2 * _TRACE_MAX_KER_CALL_NUM) {
          ext_class = _NTO_TRACE_KERCALLEXIT;
          ext_event = int_event - _TRACE_MAX_KER_CALL_NUM;
        } else if (int_event < 3 * _TRACE_MAX_KER_CALL_NUM) {
          ext_class = _NTO_TRACE_KERCALLINT;
          ext_event = int_event - 2 * _TRACE_MAX_KER_CALL_NUM;
        } else {
          Warning() << "Unknown kernel event: " << int_event;
        }

        /* Add _NTO_TRACE_KERCALL64 to the external event if it was set for the
         * internal event. */
        if (event_64) {
          ext_event = ext_event | _NTO_TRACE_KERCALL64;
        }

        break;

      case _TRACE_PR_TH_C:
        ext_event = -1;
        if (int_event >= (2 * _TRACE_MAX_TH_STATE_NUM)) {
          ext_class = _NTO_TRACE_PROCESS;
          ext_event = 1 << ((int_event >> 6) - 1);
        } else if (int_event >= _TRACE_MAX_TH_STATE_NUM) {
          ext_class = _NTO_TRACE_VTHREAD;
          ext_event = 1 << (int_event - _TRACE_MAX_TH_STATE_NUM);
        } else {
          ext_class = _NTO_TRACE_THREAD;
          ext_event = 1 << int_event;
        }
        break;

      case _TRACE_SEC_C:
        ext_class = _NTO_TRACE_SEC;
        ext_event = int_event;
        break;

      case _TRACE_SYSTEM_C:
        ext_class = _NTO_TRACE_SYSTEM;
        ext_event = int_event;
        break;

      case _TRACE_USER_C:
        ext_class = _NTO_TRACE_USER;
        ext_event = int_event;
        break;

      default:
        Warning() << "Unknown class: " << int_class;
    }
  }
};

template <typename T>
void to_json(nlohmann::json& j, const TraceEvent<T>& e) {  // NOLINT
  uint32_t ext_class;
  uint32_t ext_event;
  TraceEvent<T>::ToExt(_NTO_TRACE_GETEVENT_C(e.event_->header),
                       _NTO_TRACE_GETEVENT(e.event_->header), ext_class,
                       ext_event);
  j = nlohmann::json{{"ts", e.event_->data[0]},
                     {"cpu", _NTO_TRACE_GETCPU(e.event_->header)},
                     {"class", ClassName[ext_class]},
                     {"event", ext_event}};
};
// NOLINTEND
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_EVENT_H_
