#include "proc_info.h"

#include <cstdint>
#include <memory>
#include <utility>

namespace coding_nerd::boot_perf {
ProcInfo& ProcInfo::Singleton() {
  static ProcInfo instance;

  return instance;
}

void ProcInfo::Dead(const uint32_t& pid) {
  running_processes_.emplace(pid, std::make_shared<ProcessInfo>());
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