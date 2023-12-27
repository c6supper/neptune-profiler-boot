#ifndef TRACE_EVENT_H_
#define TRACE_EVENT_H_

#include <cstdint>
#include <iostream>
#include <memory>

#include "trace_type.h"

namespace coding_nerd::boot_perf {

template <typename T>
class TraceEvent {
  using class_type = uint32_t;
  using event_type = uint32_t;
  using cpu_type = uint32_t;

 public:
  TraceEvent() = delete;
  TraceEvent(TraceEvent& other) = delete;
  TraceEvent& operator=(TraceEvent& other) = delete;
  TraceEvent(TraceEvent&& other) = default;
  explicit TraceEvent(std::shared_ptr<T>&& event) : event_(std::move(event)) {
    toExt(_NTO_TRACE_GETEVENT_C(event->header),
          _NTO_TRACE_GETEVENT(event->header), &class_, &type_);
    cpu_ = _NTO_TRACE_GETCPU(event->header);
    timestamp_ = _NTO_TRACE_GETCPU(event->data[0]);
  };

 private:
  const std::shared_ptr<T> event_;
  class_type class_;
  class_type type_;
  cpu_type cpu_;
  uint32_t timestamp_;

  static void ToExt(const class_type int_class, event_type int_event,
                    class_type& ext_class, event_type& ext_event) {
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
            std::cerr << "Unknown Interrupt event: " << int_event << "\n";
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
          std::cerr << "Unknown kernel event: " << int_event << "\n";
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
        std::cerr << "Unknown class: " << int_class << "\n";
    }
  }
};
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_EVENT_H_
