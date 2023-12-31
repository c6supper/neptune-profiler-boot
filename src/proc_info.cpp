#include "proc_info.h"

namespace coding_nerd::boot_perf {
ProcInfo& ProcInfo::Singleton() {
  static ProcInfo instance;

  return instance;
}
}  // namespace coding_nerd::boot_perf