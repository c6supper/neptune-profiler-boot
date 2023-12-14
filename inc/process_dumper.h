#ifndef PROCESS_DUMPER_H_
#define PROCESS_DUMPER_H_

#include "runnable.h"

namespace coding_nerd::boot_perf {
template <typename ostream>
class ProcessDumper : public Runnable {
  using type = ProcessDumper<ARG...>;
};
}  // namespace coding_nerd::boot_perf

#endif  // PROCESS_DUMPER_H_
