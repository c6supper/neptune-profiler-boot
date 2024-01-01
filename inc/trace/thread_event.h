#ifndef THREAD_EVENT_H_
#define THREAD_EVENT_H_

#include <boost/format.hpp>
#include <boost/format/format_fwd.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "event_factory.h"
#include "json.hpp"
#include "logger.h"
#include "proc_info.h"
#include "trace/trace_clock.h"
#include "trace_event.h"
#include "trace_type.h"
namespace coding_nerd::boot_perf {

template <typename T>
struct ThreadEvent : TraceEvent<T> {
  ThreadEvent() = delete;
  ThreadEvent(ThreadEvent& other) = delete;
  ThreadEvent& operator=(ThreadEvent& other) = delete;
  ThreadEvent(ThreadEvent&& other) = delete;
  ~ThreadEvent() override = default;
  explicit ThreadEvent(std::shared_ptr<T>& event,
                       std::shared_ptr<TraceClock>& clock)
      : TraceEvent<T>(event, clock){};

  static void ToJson(nlohmann::json& j, std::vector<T>& e,
                     const TraceClock& trace_clock) {
    TraceEvent<T>::ToJson(j, e, trace_clock);
    VerboseLogger() << "ThreadEvent" << j;
  };

  static void ToFtrace(std::string& trace, std::vector<T>& e,
                       const TraceClock& trace_clock) {
    uint32_t ext_class;
    uint32_t ext_event;
    TraceEvent<T>::ToExt(_NTO_TRACE_GETEVENT_C(e[0].header),
                         _NTO_TRACE_GETEVENT(e[0].header), ext_class,
                         ext_event);

    switch (ext_event) {
      case _NTO_TRACE_THRUNNING: {
        auto& thread_info = GetRunningThread(e[0].data[1], e[0].data[2]);
        if (!thread_info) {
          thread_info.reset(new ThreadInfo());
        }
        thread_info->tgid = e[0].data[1];
        thread_info->tid = e[0].data[2];
        thread_info->priority = e[1].data[1];
        thread_info->policy = e[1].data[2];
        thread_info->cpu = _NTO_TRACE_GETCPU(e[0].header);
        thread_info->state = STATE_RUNNING;

        auto& cpu_thread_info = GetCPURunningThread(thread_info->cpu);
        if (!cpu_thread_info) {
          cpu_thread_info = thread_info;
          break;
        }
        auto& cpu_process_info = GetRunningProcess(cpu_thread_info->tgid);
        auto& next_process_info = GetRunningProcess(thread_info->tgid);

        const uint32_t sec =
            trace_clock.NanoSinceBootFromCycle(e[0].data[0]).count() / 1000 /
            1000 / 1000;
        const uint32_t ms =
            trace_clock.NanoSinceBootFromCycle(e[0].data[0]).count() %
            (1000 * 1000 * 1000);
        // clang-format off
        trace += boost::str(
            boost::format("%s-%d (-----) [00%1d] .... %d.%d: \
sched_switch: prev_comm=%s prev_pid=%d \
prev_prio=%d prev_state=%d ==> next_comm=%s \
next_pid=%d next_prio=%d\\n ") %
            cpu_process_info->name % cpu_process_info->pid %
            _NTO_TRACE_GETCPU(e[0].header) % sec % ms % cpu_process_info->name %
            cpu_process_info->pid % cpu_thread_info->priority %
            cpu_thread_info->state % next_process_info->name %
            next_process_info->pid % thread_info->priority);
        // clang-format on
        cpu_thread_info = thread_info;
        InfoLogger() << "_NTO_TRACE_THRUNNING " << *thread_info;
        break;
      }
      case _NTO_TRACE_THCREATE: {
        auto& thread_info = GetRunningThread(e[0].data[1], e[0].data[2]);
        if (!thread_info) {
          thread_info.reset(new ThreadInfo());
        }
        thread_info->tgid = e[0].data[1];
        thread_info->tid = e[0].data[2];
        thread_info->cpu = _NTO_TRACE_GETCPU(e[0].header);
        InfoLogger() << "_NTO_TRACE_THCREATE " << *thread_info;
        break;
      }

      default:
        break;
    }
  };
};

}  // namespace coding_nerd::boot_perf
#endif  // THREAD_EVENT_H_
