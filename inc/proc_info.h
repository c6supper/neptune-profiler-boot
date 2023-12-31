#ifndef PROC_INFO_H_
#define PROC_INFO_H_

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

#include "trace_type.h"

#define GetRunningThread(pid, tid)              \
  coding_nerd::boot_perf::ProcInfo::Singleton() \
      .running_processes_[pid]                  \
      ->running_threads_[tid]

#define GetRunningProcess(pid) \
  coding_nerd::boot_perf::ProcInfo::Singleton().running_processes_[pid]

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
    return os << "thread tgid: " << thread.tgid << ", "
              << "tid: " << thread.tid << ", "
              << "priority: " << thread.priority << ", "
              << "policy: " << thread.policy << ", "
              << "state: " << thread.state << ", "
              << "name: " << thread.name << ", "
              << "cpu: " << thread.cpu << "\n";
  };
};

struct ProcessInfo {
  uint32_t pid;
  uint32_t ppid;
  _PROCESS_STATE state;
  std::string name;

  std::unordered_map<uint32_t, std::shared_ptr<ThreadInfo>> running_threads_;
  std::unordered_map<uint32_t, std::shared_ptr<ThreadInfo>> destroyed_threads_;

  friend std::ostream& operator<<(std::ostream& os,
                                  ProcessInfo const& process) {
    return os << "process pid: " << process.pid << ", "
              << "ppid: " << process.ppid << ", "
              << "state: " << process.state << ", "
              << "name: " << process.name << "\n";
  };
};

struct ProcInfo {
  std::unordered_map<uint32_t, std::shared_ptr<ProcessInfo>> running_processes_;
  std::unordered_map<uint32_t, std::shared_ptr<ProcessInfo>>
      destroyed_processes_;
  std::unordered_map<uint32_t, std::shared_ptr<ThreadInfo>> cpu_;

  static ProcInfo& Singleton();
};

}  // namespace coding_nerd::boot_perf
#endif  // PROC_INFO_H_
