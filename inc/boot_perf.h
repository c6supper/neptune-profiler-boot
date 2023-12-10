#ifndef BOOT_PERF_H_
#define BOOT_PERF_H_

enum {
  kBootPerfVersionMajor = 4,  // incompatible API changes
  kBootPerfVersionMinor = 3,  // backwards-compatible changes
  kBootPerfVersionPatch = 11  // backwards-compatible bug fixes
};

#include <chrono>  // high_resolution_clock
#include <type_traits>

#if defined(BOOT_PERF_LOG_ENABLED)
#include <iostream>
#define BOOT_PERF_LOG(x)                                                    \
  do {                                                                      \
    std::cout << __FUNCTION__ << "@" << __LINE__ << ": " << x << std::endl; \
  } while (0)
#else
#define BOOT_PERF_LOG(x) \
  do {                   \
  } while (0)
#endif

namespace coding_nerd::boot_perf {
using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                 std::chrono::high_resolution_clock,
                                 std::chrono::steady_clock>;
}  // namespace coding_nerd::boot_perf
#endif  // BOOT_PERF_H_
