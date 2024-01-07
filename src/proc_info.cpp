#include "proc_info.h"

#include <cstdint>
#include <memory>
#include <ostream>
#include <utility>

namespace coding_nerd::boot_perf {
ProcInfo& ProcInfo::Singleton() {
  static ProcInfo instance;

  return instance;
}

void ProcInfo::Dead(const uint32_t& pid) {
  running_processes_.emplace(pid, std::make_shared<ProcessInfo>());
}

void ProcInfo::ToMeta(std::ostream& os) {
  os << R"("traceEvents":[)";

  for (auto process = running_processes_.begin();
       process != running_processes_.end(); ++process) {
    if (process->second) {
      if ((*process->second).ppid == 0 && (*process->second).pid == 0) {
        continue;
      }
      for (auto thread = process->second->running_threads_.begin();
           thread != process->second->running_threads_.end(); ++thread) {
        if (thread->second) {
          if ((*thread->second).tid == 0 && (*thread->second).tgid == 0) {
            continue;
          }
          std::string name = process->second->name;
          if (!thread->second->name.empty()) {
            name = thread->second->name;
          }
          os << R"({"args":{"name":")" << name
             << R"("},"cat":"__metadata","name":"thread_name","ph":"M","pid":)"
             << thread->second->tgid << R"(,"tid":)" << thread->second->tid
             << R"(,"ts":0})";
          if ((process->second->running_threads_.end() != std::next(thread)) ||
              (running_processes_.end() != std::next(process))) {
            os << ",";
          }
        }
      }
    }
  }

  for (auto process = running_processes_.begin();
       process != running_processes_.end(); ++process) {
    if (process->second) {
      if ((*process->second).ppid == 0 && (*process->second).pid == 0) {
        continue;
      }
      os << R"({"args":{"name":")" << process->second->name
         << R"("},"cat":"__metadata","name":"process_name","ph":"M","pid":)"
         << process->second->pid << R"(,"tid":0,"ts":0})";
      if (running_processes_.end() != std::next(process)) {
        os << ",";
      }
    }
  }
  os << R"(])";
}

std::shared_ptr<ProcessInfo>& ProcInfo::operator[](const uint32_t& pid) {
  if (auto search = running_processes_.find(pid);
      search != running_processes_.end()) {
    return search->second;
  }
  return running_processes_.emplace(pid, std::shared_ptr<ProcessInfo>(nullptr))
      ->second;
}

void ProcessInfo::Dead(const uint32_t& tid) {
  running_threads_.emplace(tid, std::make_shared<ThreadInfo>());
}

std::shared_ptr<ThreadInfo>& ProcessInfo::operator[](const uint32_t& tid) {
  if (auto search = running_threads_.find(tid);
      search != running_threads_.end()) {
    return search->second;
  }
  return running_threads_.emplace(tid, std::shared_ptr<ThreadInfo>(nullptr))
      ->second;
}

}  // namespace coding_nerd::boot_perf