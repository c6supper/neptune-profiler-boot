#ifndef TRACE_EVENT_H_
#define TRACE_EVENT_H_

#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>

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
            ext_class = _NTO_TRACE_UNKOWN;
            ext_event = _NTO_TRACE_MAX_CODES;
            VerboseLogger() << "Unknown Interrupt event: " << int_event;
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
          ext_class = _NTO_TRACE_UNKOWN;
          ext_event = _NTO_TRACE_MAX_CODES;
          VerboseLogger() << "Unknown kernel event: " << int_event;
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
        ext_class = _NTO_TRACE_UNKOWN;
        ext_event = _NTO_TRACE_MAX_CODES;
        VerboseLogger() << "Unknown class: " << int_class;
    }
  }

  static void ToExt(const T& e, uint32_t& ext_class, uint32_t& ext_event) {
    ToExt(_NTO_TRACE_GETEVENT_C(e.header), _NTO_TRACE_GETEVENT(e.header),
          ext_class, ext_event);
  };
  static void ToExt(std::vector<T>& e, uint32_t& ext_class,
                    uint32_t& ext_event) {
    ToExt(_NTO_TRACE_GETEVENT_C(e[0].header), _NTO_TRACE_GETEVENT(e[0].header),
          ext_class, ext_event);
  };

  static bool IsValid(const T& e) {
    uint32_t ext_class;
    uint32_t ext_event;
    ToExt(e, ext_class, ext_event);
    if (ext_class == _NTO_TRACE_UNKOWN || ext_event == _NTO_TRACE_MAX_CODES) {
      return false;
    }
    return true;
  }
  static std::string ToExtString(uint32_t ext_class, uint32_t ext_event) {
    static std::unordered_map<uint32_t, std::string> ext_event_to_name = {
        {(0x00000000u), "_NTO_TRACE_EMPTYEVENT"},
        {(0x00000001u << 0), "_NTO_TRACE_CONTROLTIME"},
        {(0x00000001u << 1), "_NTO_TRACE_CONTROLBUFFER"},
        {(0x00000000u), "_NTO_TRACE_KERCALLFIRST"},
        {(__KER_BAD), "_NTO_TRACE_KERCALLLAST"},
        {(0x00000001u << 9), "_NTO_TRACE_KERCALL64"},
        {(0x00000000u), "_NTO_TRACE_INTFIRST"},
        {(0xffffffffu), "_NTO_TRACE_INTLAST"},
        {(0x00000001u << 0), "_NTO_TRACE_PROCCREATE"},
        {(0x00000001u << 1), "_NTO_TRACE_PROCDESTROY"},
        {(0x00000001u << 2), "_NTO_TRACE_PROCCREATE_NAME"},
        {(0x00000001u << 3), "_NTO_TRACE_PROCDESTROY_NAME"},
        {(0x00000001u << 4), "_NTO_TRACE_PROCTHREAD_NAME"},
        {(0x00000001u << STATE_DEAD), "_NTO_TRACE_THDEAD"},
        {(0x00000001u << STATE_RUNNING), "_NTO_TRACE_THRUNNING"},
        {(0x00000001u << STATE_READY), "_NTO_TRACE_THREADY"},
        {(0x00000001u << STATE_STOPPED), "_NTO_TRACE_THSTOPPED"},
        {(0x00000001u << STATE_SEND), "_NTO_TRACE_THSEND"},
        {(0x00000001u << STATE_RECEIVE), "_NTO_TRACE_THRECEIVE"},
        {(0x00000001u << STATE_REPLY), "_NTO_TRACE_THREPLY"},
        {(0x00000001u << STATE_STACK), "_NTO_TRACE_THSTACK"},
        {(0x00000001u << STATE_WAITTHREAD), "_NTO_TRACE_THWAITTHREAD"},
        {(0x00000001u << STATE_WAITPAGE), "_NTO_TRACE_THWAITPAGE"},
        {(0x00000001u << STATE_SIGSUSPEND), "_NTO_TRACE_THSIGSUSPEND"},
        {(0x00000001u << STATE_SIGWAITINFO), "_NTO_TRACE_THSIGWAITINFO"},
        {(0x00000001u << STATE_NANOSLEEP), "_NTO_TRACE_THNANOSLEEP"},
        {(0x00000001u << STATE_MUTEX), "_NTO_TRACE_THMUTEX"},
        {(0x00000001u << STATE_CONDVAR), "_NTO_TRACE_THCONDVAR"},
        {(0x00000001u << STATE_JOIN), "_NTO_TRACE_THJOIN"},
        {(0x00000001u << STATE_INTR), "_NTO_TRACE_THINTR"},
        {(0x00000001u << STATE_SEM), "_NTO_TRACE_THSEM"},
        {(0x00000001u << STATE_WAITCTX), "_NTO_TRACE_THWAITCTX"},
        {(0x00000001u << STATE_NET_SEND), "_NTO_TRACE_THNET_SEND"},
        {(0x00000001u << STATE_NET_REPLY), "_NTO_TRACE_THNET_REPLY"},
        {(0x00000001u << _TRACE_THREAD_CREATE), "_NTO_TRACE_THCREATE"},
        {(0x00000001u << _TRACE_THREAD_DESTROY), "_NTO_TRACE_THDESTROY"},
        {(0x00000001u << STATE_DEAD), "_NTO_TRACE_VTHDEAD"},
        {(0x00000001u << STATE_RUNNING), "_NTO_TRACE_VTHRUNNING"},
        {(0x00000001u << STATE_READY), "_NTO_TRACE_VTHREADY"},
        {(0x00000001u << STATE_STOPPED), "_NTO_TRACE_VTHSTOPPED"},
        {(0x00000001u << STATE_SEND), "_NTO_TRACE_VTHSEND"},
        {(0x00000001u << STATE_RECEIVE), "_NTO_TRACE_VTHRECEIVE"},
        {(0x00000001u << STATE_REPLY), "_NTO_TRACE_VTHREPLY"},
        {(0x00000001u << STATE_STACK), "_NTO_TRACE_VTHSTACK"},
        {(0x00000001u << STATE_WAITTHREAD), "_NTO_TRACE_VTHWAITTHREAD"},
        {(0x00000001u << STATE_WAITPAGE), "_NTO_TRACE_VTHWAITPAGE"},
        {(0x00000001u << STATE_SIGSUSPEND), "_NTO_TRACE_VTHSIGSUSPEND"},
        {(0x00000001u << STATE_SIGWAITINFO), "_NTO_TRACE_VTHSIGWAITINFO"},
        {(0x00000001u << STATE_NANOSLEEP), "_NTO_TRACE_VTHNANOSLEEP"},
        {(0x00000001u << STATE_MUTEX), "_NTO_TRACE_VTHMUTEX"},
        {(0x00000001u << STATE_CONDVAR), "_NTO_TRACE_VTHCONDVAR"},
        {(0x00000001u << STATE_JOIN), "_NTO_TRACE_VTHJOIN"},
        {(0x00000001u << STATE_INTR), "_NTO_TRACE_VTHINTR"},
        {(0x00000001u << STATE_SEM), "_NTO_TRACE_VTHSEM"},
        {(0x00000001u << STATE_WAITCTX), "_NTO_TRACE_VTHWAITCTX"},
        {(0x00000001u << STATE_NET_SEND), "_NTO_TRACE_VTHNET_SEND"},
        {(0x00000001u << STATE_NET_REPLY), "_NTO_TRACE_VTHNET_REPLY"},
        {(0x00000001u << _TRACE_THREAD_CREATE), "_NTO_TRACE_VTHCREATE"},
        {(0x00000001u << _TRACE_THREAD_DESTROY), "_NTO_TRACE_VTHDESTROY"},
        {(0x00000000u), "_NTO_TRACE_USERFIRST"},
        {(0x000003ffu), "_NTO_TRACE_USERLAST"},
        {(0x00000000), "_NTO_TRACE_QUIPFIRST"},
        {(0x000003ff), "_NTO_TRACE_QUIPLAST"},
        {(0x00000000), "_NTO_TRACE_SECFIRST"},
        {(0x000003ff), "_NTO_TRACE_SECLAST"},
        {(0x00000000u), "_NTO_TRACE_QVMFIRST"},
        {(0x000003ffu), "_NTO_TRACE_QVMLAST"},
        {(0x00000001u), "_NTO_TRACE_SYS_RESERVED"},
        {(0x00000002u), "_NTO_TRACE_SYS_PATHMGR"},
        {(0x00000003u), "_NTO_TRACE_SYS_APS_NAME"},
        {(0x00000004u), "_NTO_TRACE_SYS_APS_BUDGETS"},
        {(0x00000005), "_NTO_TRACE_SYS_APS_BNKR"},
        {(0x00000006u), "_NTO_TRACE_SYS_MMAP"},
        {(0x00000007u), "_NTO_TRACE_SYS_MUNMAP"},
        {(0x00000008u), "_NTO_TRACE_SYS_MAPNAME"},
        {(0x00000009u), "_NTO_TRACE_SYS_ADDRESS"},
        {(0x0000000au), "_NTO_TRACE_SYS_FUNC_ENTER"},
        {(0x0000000bu), "_NTO_TRACE_SYS_FUNC_EXIT"},
        {(0x0000000cu), "_NTO_TRACE_SYS_SLOG"},
        {(0x0000000du), "_NTO_TRACE_SYS_DEFRAG_START"},
        {(0x0000000eu), "_NTO_TRACE_SYS_RUNSTATE"},
        {(0x0000000fu), "_NTO_TRACE_SYS_POWER"},
        {(0x00000010u), "_NTO_TRACE_SYS_IPI"},
        {(0x00000011u), "_NTO_TRACE_SYS_PAGEWAIT"},
        {(0x00000012u), "_NTO_TRACE_SYS_TIMER"},
        {(0x00000013u), "_NTO_TRACE_SYS_DEFRAG_END"},
        {(0x00000014u), "_NTO_TRACE_SYS_PROFILE"},
        {(0x00000015u), "_NTO_TRACE_SYS_MAPNAME_64"},
        {(_NTO_TRACE_SYS_MAPNAME_64), "_NTO_TRACE_SYS_LAST"},
        {(_NTO_TRACE_SYS_IPI | _NTO_TRACE_KERCALL64), "_NTO_TRACE_SYS_IPI_64"},
        {(_NTO_TRACE_SYS_PROFILE | _NTO_TRACE_KERCALL64),
         "_NTO_TRACE_SYS_PROFILE_64"},
        {(0xFFFFFFFFu), "_NTO_TRACE_UNKOWN"}};
    std::string ext = "Extern class=";
    if (ext_class > _NTO_TRACE_UNKOWN) {
      ext_class = _NTO_TRACE_UNKOWN;
    }
    ext += ClassName[ext_class];

    if (ext_event_to_name.find(ext_event) == ext_event_to_name.end()) {
      ext_event = 0xFFFFFFFFu;
    }
    ext += ",event=" + ext_event_to_name[ext_event];
    return ext;
  }

  static void Dump(const T& event, const TraceClock& trace_clock) {
    printf("t:%8ld ns CPU:%02d 0x%-8x:0x%-8x",
           trace_clock.NanoSinceBootFromCycle(event.data[0]).count(),
           _NTO_TRACE_GETCPU(event.header), event.data[1], event.data[2]);
    uint32_t ext_class;
    uint32_t ext_event;
    ToExt(event, ext_class, ext_event);
    printf(", %-40s,", ToExtString(ext_class, ext_event).c_str());

    switch (_TRACE_GET_STRUCT(event.header)) {
      case _TRACE_STRUCT_CC:
        printf("%-s\n", " _TRACE_STRUCT_CC");
        break;
      case _TRACE_STRUCT_CB:
        printf("%-s\n", " _TRACE_STRUCT_CB");
        break;
      case _TRACE_STRUCT_S:
        printf("%-s\n", " _TRACE_STRUCT_S");
        break;
      case _TRACE_STRUCT_CE:
        printf("%-s\n", " _TRACE_STRUCT_CE");
        break;
      default:
        break;
    }
  }

  // static void ToJson(nlohmann::json& j, T& e, const TraceClock& trace_clock)
  // {
  //   uint32_t ext_class;
  //   uint32_t ext_event;
  //   TraceEvent<T>::ToExt(e, ext_class, ext_event);
  //   j = nlohmann::json{
  //       {"ts", trace_clock.NanoSinceBootFromCycle(e.data[0]).count()},
  //       {"cpu", _NTO_TRACE_GETCPU(e.header)},
  //       {"class", ClassName[ext_class]},
  //       {"event", ext_event}};
  // }

  static void ToJson(nlohmann::json& j, std::vector<T>& e,
                     const TraceClock& trace_clock) {
    uint32_t ext_class;
    uint32_t ext_event;
    TraceEvent<T>::ToExt(_NTO_TRACE_GETEVENT_C(e[0].header),
                         _NTO_TRACE_GETEVENT(e[0].header), ext_class,
                         ext_event);

    j = nlohmann::json{
        {"ts", trace_clock.NanoSinceBootFromCycle(e[0].data[0]).count()},
        {"cpu", _NTO_TRACE_GETCPU(e[0].header)},
        {"class", ClassName[ext_class]},
        {"event", ext_event}};
  };
};

// NOLINTEND
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_EVENT_H_
