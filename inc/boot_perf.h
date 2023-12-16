#ifndef BOOT_PERF_H_
#define BOOT_PERF_H_

enum {
  kBootPerfVersionMajor = 4,  // incompatible API changes
  kBootPerfVersionMinor = 3,  // backwards-compatible changes
  kBootPerfVersionPatch = 11  // backwards-compatible bug fixes
};

#include <atomic>
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include "compiler.h"
#include "config.h"
#include "logger.h"
#include "process_dumper.h"
#include "runnable.h"

#define BootPerfContext() coding_nerd::boot_perf::BootPerf::Instance()

namespace coding_nerd::boot_perf {

class BootPerf final {
 public:
  BootPerf() {
    std::ofstream ps("/tmp/proc_ps.log");
    // stat_.open("/tmp/proc_stat.log");
    // header_.open("/tmp/header");

    registerRunnable(
        std::make_shared<ProcessDumper<std::ofstream>>(std::move(ps)));

    BOOT_PERF_LOG("BootPerf Initialized!");
  }
  ~BootPerf() = default;
  BootPerf(BootPerf&& other) noexcept = delete;
  BootPerf& operator=(BootPerf&& other) = delete;
  BootPerf(BootPerf const& other) = delete;
  BootPerf& operator=(BootPerf const& other) = delete;

  template <typename Rep = int32_t, typename Period = std::milli>
  void run(const std::chrono::duration<Rep, Period>& _period) {
    while (running_) {
      const std::lock_guard<std::mutex> guard(protector_);
      auto iterator = runnable_.begin();
      for (; iterator != runnable_.end(); iterator++) {
        (*iterator)->run();
      }
      std::this_thread::sleep_for(_period);
    }
  }

  static auto& Instance() {
    static BootPerf instance;

    return instance;
  }

  template <typename Rep = int32_t, typename Period = std::milli>
  static void Start(const std::chrono::duration<Rep, Period>& _period) {
    Instance().running_ = true;
    Instance().collector_ =
        std::thread([&] { Instance().run(std::move(_period)); });
  }

  static void Stop() {
    Instance().running_ = false;
    Instance().collector_.join();
  }

 private:
  void registerRunnable(std::shared_ptr<Runnable>&& runnable) {
    const std::lock_guard<std::mutex> guard(protector_);
    runnable_.push_back(std::move(runnable));
  };

  std::vector<std::shared_ptr<Runnable>> runnable_;
  std::mutex protector_;
  std::atomic<bool> running_;
  std::thread collector_;
};
}  // namespace coding_nerd::boot_perf

#endif  // BOOT_PERF_H_
