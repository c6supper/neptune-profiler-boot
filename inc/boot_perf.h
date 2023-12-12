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
CODING_NERD_BOOT_PERF(IGNORE_PADDED_PUSH)
class BootPerf final {
 public:
  BootPerf();

  BootPerf(BootPerf&& other) noexcept;
  BootPerf& operator=(BootPerf&& other) noexcept(
      CODING_NERD_BOOT_PERF_PRIVATE_NOEXCEPT_STRING_MOVE());
  BootPerf(BootPerf const& other);
  BootPerf& operator=(BootPerf const& other);

  template <typename Op>
  CODING_NERD_BOOT_PERF(NOINLINE)
  BootPerf& run(Op&& op);
};
}  // namespace coding_nerd::boot_perf
CODING_NERD_BOOT_PERF(IGNORE_PADDED_POP)

CODING_NERD_BOOT_PERF(IGNORE_PADDED_PUSH)
namespace coding_nerd::boot_perf {
BootPerf::BootPerf() = default;

BootPerf::BootPerf(BootPerf&&) noexcept = default;
BootPerf& BootPerf::operator=(BootPerf&&) noexcept(
    CODING_NERD_BOOT_PERF_PRIVATE_NOEXCEPT_STRING_MOVE()) = default;
BootPerf::BootPerf(BootPerf const&) = default;
BootPerf& BootPerf::operator=(BootPerf const&) = default;

template <typename Op>
BootPerf& BootPerf::run(Op&& op) {
  return run(std::forward<Op>(op));
}
};  // namespace coding_nerd::boot_perf
CODING_NERD_BOOT_PERF(IGNORE_PADDED_POP)
#endif  // BOOT_PERF_H_
