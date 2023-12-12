#ifndef BOOT_PERF_H_
#define BOOT_PERF_H_

enum {
  kBootPerfVersionMajor = 4,  // incompatible API changes
  kBootPerfVersionMinor = 3,  // backwards-compatible changes
  kBootPerfVersionPatch = 11  // backwards-compatible bug fixes
};

#include <chrono>  // high_resolution_clock
#include <type_traits>

#include "compiler.h"

namespace coding_nerd::boot_perf {
using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                 std::chrono::high_resolution_clock,
                                 std::chrono::steady_clock>;

class BootPerf final {
 public:
  BootPerf();

  BootPerf(BootPerf&& other) noexcept = default;
  BootPerf& operator=(BootPerf&& other) noexcept(
      CODING_NERD_BOOT_PERF_PRIVATE_NOEXCEPT_STRING_MOVE()) = default;
  BootPerf(BootPerf const& other) = default;
  BootPerf& operator=(BootPerf const& other) = default;

  template <typename Op>
  CODING_NERD_BOOT_PERF(NOINLINE)
  BootPerf& run(Op&& op) {
    return run(std::forward<Op>(op));
  }
};
}  // namespace coding_nerd::boot_perf

#endif  // BOOT_PERF_H_