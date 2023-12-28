#ifndef TRACE_EVENT_H_
#define TRACE_EVENT_H_

#include <cstdint>
#include <iostream>
#include <memory>

#include "json.hpp"
#include "logger.h"
#include "trace_clock.h"
#include "trace_type.h"

namespace coding_nerd::boot_perf {

template <typename T>
struct TraceEvent {
  std::shared_ptr<T> event_;
  std::shared_ptr<TraceClock> clock_;
  TraceEvent() = delete;
  TraceEvent(TraceEvent& other) = delete;
  TraceEvent& operator=(TraceEvent& other) = delete;
  TraceEvent(TraceEvent&& other) = delete;
  virtual ~TraceEvent() = default;
  explicit TraceEvent(std::shared_ptr<T>& event,
                      std::shared_ptr<TraceClock>& trace_clock_)
      : event_(event), clock_(trace_clock_){};  // NOLINT
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
            WarningLogger() << "Unknown Interrupt event: " << int_event;
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
          WarningLogger() << "Unknown kernel event: " << int_event;
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
        WarningLogger() << "Unknown class: " << int_class;
    }
  }

  static void ToExt(const T& e, uint32_t& ext_class, uint32_t& ext_event) {
    ToExt(_NTO_TRACE_GETEVENT_C(e.header), _NTO_TRACE_GETEVENT(e.header),
          ext_class, ext_event);
  };
  static void Dump(const T& event, const TraceClock& trace_clock) {
    printf("t:%8ld ns CPU:%02d 0x%-8x:0x%-8x",
           trace_clock.NanoSinceBootFromCycle(event.data[0]).count(),
           _NTO_TRACE_GETCPU(event.header), event.data[1], event.data[2]);
    switch (_TRACE_GET_STRUCT(event.header)) {
      case _TRACE_STRUCT_CC:
        printf("%-s\n", "_TRACE_STRUCT_CC");
        break;
      case _TRACE_STRUCT_CB:
        printf("%-s\n", "_TRACE_STRUCT_CB");
        break;
      case _TRACE_STRUCT_S:
        printf("%-s\n", "_TRACE_STRUCT_S");
        break;
      case _TRACE_STRUCT_CE:
        printf("%-s\n", "_TRACE_STRUCT_CE");
        break;
      default:
        break;
    }
  }

  static void ToJson(nlohmann::json& j, const T& e,
                     const TraceClock& trace_clock) {
    uint32_t ext_class;
    uint32_t ext_event;
    TraceEvent<T>::ToExt(e, ext_class, ext_event);
    j = nlohmann::json{
        {"ts", trace_clock.NanoSinceBootFromCycle(e.data[0]).count()},
        {"cpu", _NTO_TRACE_GETCPU(e.header)},
        {"class", ClassName[ext_class]},
        {"event", ext_event}};
  }

  static void ToJson(
      nlohmann::json& j,
      typename std::enable_if<std::is_array<T>::value, const T&> e,
      const TraceClock& trace_clock) {
    uint32_t ext_class;
    uint32_t ext_event;
    TraceEvent<T>::ToExt(_NTO_TRACE_GETEVENT_C(e[0]->header),
                         _NTO_TRACE_GETEVENT(e[0]->header), ext_class,
                         ext_event);
    j = nlohmann::json{
        {"ts", trace_clock.NanoSinceBootFromCycle(e[0]->data[0]).count()},
        {"cpu", _NTO_TRACE_GETCPU(e[0]->header)},
        {"class", ClassName[ext_class]},
        {"event", ext_event}};
  };
};

// NOLINTEND
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_EVENT_H_
