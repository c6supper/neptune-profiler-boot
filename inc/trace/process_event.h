#ifndef PROCESS_EVENT_H_
#define PROCESS_EVENT_H_

#include <cstdint>
#include <memory>
#include <string>

#include "event_factory.h"
#include "json.hpp"
#include "logger.h"
#include "proc_info.h"
#include "trace/trace_clock.h"
#include "trace_event.h"
#include "trace_type.h"
#include "unused.h"
namespace coding_nerd::boot_perf {

template <typename T>
struct ProcessEvent : TraceEvent<T> {
  ProcessEvent() = delete;
  ProcessEvent(ProcessEvent& other) = delete;
  ProcessEvent& operator=(ProcessEvent& other) = delete;
  ProcessEvent(ProcessEvent&& other) = delete;
  ~ProcessEvent() override = default;
  explicit ProcessEvent(std::shared_ptr<T>& event,
                        std::shared_ptr<TraceClock>& clock)
      : TraceEvent<T>(event, clock){};

  static void ToJson(nlohmann::json& j, std::vector<T>& e,
                     const TraceClock& trace_clock) {
    TraceEvent<T>::ToJson(j, e, trace_clock);
    VerboseLogger() << "ProcessEvent" << j;
  };

  static void ToFtrace(std::string& trace, std::vector<T>& e,
                       const TraceClock& trace_clock) {
    MONUnusedParameter(trace);
    MONUnusedParameter(trace_clock);
    uint32_t ext_class;
    uint32_t ext_event;
    TraceEvent<T>::ToExt(_NTO_TRACE_GETEVENT_C(e[0].header),
                         _NTO_TRACE_GETEVENT(e[0].header), ext_class,
                         ext_event);
    char name[_TRACE_MAX_STR_SIZE];
    switch (ext_event) {
      case _NTO_TRACE_PROCCREATE_NAME: {
        for (auto i = 1; i < e.size(); i++) {
          std::copy(reinterpret_cast<char*>(&(e[i].data[1])),
                    reinterpret_cast<char*>(&(e[i].data[2])) + 4,
                    &name[(i - 1) * 8]);
        }
        auto& process = GetRunningProcess(e[0].data[2]);
        if (!process) {
          process.reset(new ProcessInfo());
        }
        process->pid = e[0].data[2];
        process->ppid = e[0].data[1];
        process->name = std::move(std::string(name));
        InfoLogger() << "_NTO_TRACE_PROCCREATE_NAME " << *process;
        break;
      }
      case _NTO_TRACE_PROCTHREAD_NAME: {
        for (auto i = 1; i < e.size(); i++) {
          std::copy(reinterpret_cast<char*>(&(e[i].data[1])),
                    reinterpret_cast<char*>(&(e[i].data[2])) + 4,
                    &name[(i - 1) * 8]);
        }
        auto& thread = GetRunningThread(e[0].data[1], e[0].data[2]);
        if (!thread) {
          thread.reset(new ThreadInfo());
        }
        thread->tid = e[0].data[2];
        thread->tgid = e[0].data[1];
        thread->name = std::move(std::string(name));
        InfoLogger() << "_NTO_TRACE_PROCTHREAD_NAME " << *thread;
        break;
      }
      case _NTO_TRACE_PROCCREATE: {
        uint32_t ppid = e[0].data[1];
        uint32_t pid = e[0].data[2];
        auto& process = GetRunningProcess(pid);
        if (!process) {
          process.reset(new ProcessInfo());
        }
        process->pid = e[0].data[2];
        process->ppid = e[0].data[1];
        InfoLogger() << "_NTO_TRACE_PROCCREATE_NAME " << *process;
        break;
      }

      default:
        break;
    }
  }
};

}  // namespace coding_nerd::boot_perf
#endif  // PROCESS_EVENT_H_
