#ifndef CLOCK_H_
#define CLOCK_H_

#include <chrono>  // high_resolution_clock
#include <fstream>
#include <iostream>
namespace coding_nerd::boot_perf {
using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                 std::chrono::high_resolution_clock,
                                 std::chrono::steady_clock>;
template <typename T>
class BootPerfClock {
 public:
  BootPerfClock() = default;
  BootPerfClock(BootPerfClock&& other) noexcept = default;
  BootPerfClock(BootPerfClock& other) = delete;
  BootPerfClock& operator=(BootPerfClock& other) = delete;

  // align with Android, 100hz, 1 jiffy = 10 ms
  static uint64_t get_uptime_jiffies() {
    double uptime;
    std::ifstream file("/proc/uptime");
    file >> uptime;

    return static_cast<uint64_t>(uptime * 100);
  };
};
}  // namespace coding_nerd::boot_perf

#endif  // CLOCK_H_
