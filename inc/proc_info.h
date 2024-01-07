#ifndef PROC_INFO_H_
#define PROC_INFO_H_

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

#include "trace_type.h"

#define GetProcMeta(os) coding_nerd::boot_perf::ProcInfo::Singleton().ToMeta(os)

#define GetProc() coding_nerd::boot_perf::ProcInfo::Singleton()

#define GetRunningThread(pid, tid) \
  (*coding_nerd::boot_perf::ProcInfo::Singleton()[pid])[tid]

#define RunningProcessDead(pid) \
  coding_nerd::boot_perf::ProcInfo::Singleton().Dead(pid)

#define GetRunningProcess(pid) \
  coding_nerd::boot_perf::ProcInfo::Singleton()[pid]

#define RunningThreadDead(pid, tid) \
  coding_nerd::boot_perf::ProcInfo::Singleton()[pid]->Dead(tid)

#define GetCPURunningThread(cpu) \
  coding_nerd::boot_perf::ProcInfo::Singleton().cpu_[cpu]

namespace coding_nerd::boot_perf {

struct InterruptInfo {
  uint32_t index;
  uint32_t pid;
  uint32_t counter;
  uint64_t duration;
};
struct ThreadInfo {
  uint32_t tid;
  uint32_t tgid;
  uint32_t priority;
  uint32_t policy;
  _THREAD_STATE state;
  std::string name;
  uint32_t cpu;
  friend std::ostream& operator<<(std::ostream& os, ThreadInfo const& thread) {
    return os << R"({"tgid":)" << thread.tgid << ","
              << R"("tid":)" << thread.tid << ","
              << R"("priority":)" << thread.priority << ","
              << R"("policy":)" << thread.policy << ","
              << R"("state":")" << task_state[thread.state] << R"(",)"
              << R"("name":")" << thread.name << R"(",)"
              << R"("cpu":)" << thread.cpu << "}";
  };
};

struct ProcessInfo {
  uint32_t pid;
  uint32_t ppid;
  _PROCESS_STATE state;
  std::string name;

  std::unordered_multimap<uint32_t, std::shared_ptr<ThreadInfo>>
      running_threads_;
  std::unordered_map<uint32_t, std::shared_ptr<ThreadInfo>> destroyed_threads_;

  void Dead(const uint32_t& tid);

  std::shared_ptr<ThreadInfo>& operator[](const uint32_t& tid);

  friend std::ostream& operator<<(std::ostream& os,
                                  ProcessInfo const& process) {
    os << R"({"pid":)" << process.pid << ","
       << R"("ppid":)" << process.ppid << ","
       << R"("state":")" << process_state[process.state] << R"(",)"
       << R"("name":")" << process.name << R"(","thread":[)";

    for (auto it = process.running_threads_.begin();
         it != process.running_threads_.end(); ++it) {
      if (it->second) {
        if ((*it->second).tid == 0 && (*it->second).tgid == 0) {
          continue;
        }
        os << *it->second;
        if (process.running_threads_.end() != std::next(it)) {
          os << ",";
        }
      }
    }

    return os << "]}";
  };
};

struct ProcInfo {
  std::unordered_multimap<uint32_t, std::shared_ptr<ProcessInfo>>
      running_processes_;
  std::unordered_map<uint32_t, std::shared_ptr<ProcessInfo>>
      destroyed_processes_;
  std::unordered_map<uint32_t, std::shared_ptr<ThreadInfo>> cpu_;

  static ProcInfo& Singleton();
  void Dead(const uint32_t& pid);
  std::shared_ptr<ProcessInfo>& operator[](const uint32_t& pid);
  void ToMeta(std::ostream& os);

  friend std::ostream& operator<<(std::ostream& os, ProcInfo const& proc) {
    os << R"({"Process": [)";
    for (auto it = proc.running_processes_.begin();
         it != proc.running_processes_.end(); ++it) {
      if (it->second) {
        if ((*it->second).ppid == 0 && (*it->second).pid == 0) {
          continue;
        }
        os << *it->second;
        if (proc.running_processes_.end() != std::next(it)) {
          os << ",";
        }
      }
    }
    return os << "]}";
  };
};

}  // namespace coding_nerd::boot_perf
#endif  // PROC_INFO_H_
