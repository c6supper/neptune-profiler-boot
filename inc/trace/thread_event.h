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
        thread_info->priority = e[0].data[3];
        thread_info->policy = e[0].data[4];
        thread_info->cpu = _NTO_TRACE_GETCPU(e[0].header);
        thread_info->state = STATE_RUNNING;

        auto& cpu_thread_info = GetCPURunningThread(thread_info->cpu);
        if (!cpu_thread_info) {
          cpu_thread_info = thread_info;
          break;
        }

        const uint32_t sec =
            trace_clock.NanoSinceBootFromCycle(e[0].data[0]).count() / 1000 /
            1000 / 1000;
        const uint32_t ms =
            trace_clock.NanoSinceBootFromCycle(e[0].data[0]).count() %
            (1000 * 1000);
        trace += boost::str(
            boost::format(
                "          %1%-%2%     (-----) [00%3%] .... %4%.%5%: sched_switch: \
                prev_comm=%6% prev_pid=%7% prev_prio=%8% prev_state=%9% ==> \
                next_comm=%10% next_pid=%11% next_prio=%12%\n") %
            cpu_thread_info->name % cpu_thread_info->tgid %
            _NTO_TRACE_GETCPU(e[0].header) % sec % ms % cpu_thread_info->name %
            cpu_thread_info->tid % cpu_thread_info->priority %
            cpu_thread_info->state % thread_info->name % thread_info->tid %
            thread_info->priority);
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
