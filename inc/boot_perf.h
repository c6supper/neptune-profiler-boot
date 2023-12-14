#ifndef BOOT_PERF_H_
#define BOOT_PERF_H_

enum {
  kBootPerfVersionMajor = 4,  // incompatible API changes
  kBootPerfVersionMinor = 3,  // backwards-compatible changes
  kBootPerfVersionPatch = 11  // backwards-compatible bug fixes
};

#include <chrono>  // high_resolution_clock
#include <functional>
#include <type_traits>
#include <vector>

#include "compiler.h"
#include "config.h"
#include "logger.h"
#include "runnable.h"

#define BootPerfContext() coding_nerd::boot_perf::BootPerf::Instance();

namespace coding_nerd::boot_perf {
using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                 std::chrono::high_resolution_clock,
                                 std::chrono::steady_clock>;

class BootPerf final {
 public:
  BootPerf() = default;
  ~BootPerf() = default;
  BootPerf(BootPerf&& other) noexcept = delete;
  BootPerf& operator=(BootPerf&& other) noexcept(
      CODING_NERD_BOOT_PERF_PRIVATE_NOEXCEPT_STRING_MOVE()) = delete;
  BootPerf(BootPerf const& other) = delete;
  BootPerf& operator=(BootPerf const& other) = delete;

  template <typename Op>
  CODING_NERD_BOOT_PERF(NOINLINE)
  BootPerf& run(Op&& op) {
    return run(std::forward<Op>(op));
  }

  static auto& Instance() {
    static BootPerf instance;
    BOOT_PERF_LOG("BootPerf Initialized!");
    return instance;
  }

  static void RegisterRunnable(Runnable&& runnable) {
    runnable_.push_back(std::move<Runnable>(runnable));
  };

 private:
  static std::vector<Runnable> runnable_;
};
}  // namespace coding_nerd::boot_perf

#endif  // BOOT_PERF_H_