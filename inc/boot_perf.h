#ifndef BOOT_PERF_H_
#define BOOT_PERF_H_

enum {
  kBootPerfVersionMajor = 4,  // incompatible API changes
  kBootPerfVersionMinor = 3,  // backwards-compatible changes
  kBootPerfVersionPatch = 11  // backwards-compatible bug fixes
};

#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <type_traits>
#include <vector>

#include "compiler.h"
#include "config.h"
#include "logger.h"
#include "process_dumper.h"
#include "runnable.h"

#define BootPerfContext()                         \
  do {                                            \
    coding_nerd::boot_perf::BootPerf::Instance(); \
  } while (0)

namespace coding_nerd::boot_perf {

class BootPerf final {
 public:
  BootPerf() {
    std::ofstream ps("/tmp/proc_ps.log");
    // stat_.open("/tmp/proc_stat.log");
    // header_.open("/tmp/header");

    registerRunnable(
        std::make_shared<ProcessDumper<std::ofstream>>(std::move(ps)));
  }
  ~BootPerf() = default;
  BootPerf(BootPerf&& other) noexcept = delete;
  BootPerf& operator=(BootPerf&& other) = delete;
  BootPerf(BootPerf const& other) = delete;
  BootPerf& operator=(BootPerf const& other) = delete;

  template <typename Op>
  CODING_NERD_BOOT_PERF(NOINLINE)
  BootPerf& run(Op&& op) {
    return run(std::forward<Op>(op));

    while (true) {
      std::unique_lock<std::mutex> lock(g_bootcharting_finished_mutex);
      g_bootcharting_finished_cv.wait_for(lock, 200ms);
      if (g_bootcharting_finished) break;

      log_file(&*stat_log, "/proc/stat");
      log_file(&*disk_log, "/proc/diskstats");
      log_processes(&*proc_log);
    }
  }

  static auto& Instance() {
    static BootPerf instance;
    BOOT_PERF_LOG("BootPerf Initialized!");
    const std::lock_guard<std::mutex> guard(instance.protector_);

    return instance;
  }

 private:
  void registerRunnable(std::shared_ptr<Runnable>&& runnable) {
    runnable_.push_back(std::move(runnable));
  };

  std::vector<std::shared_ptr<Runnable>> runnable_;
  std::mutex protector_;
};
}  // namespace coding_nerd::boot_perf

#endif  // BOOT_PERF_H_
