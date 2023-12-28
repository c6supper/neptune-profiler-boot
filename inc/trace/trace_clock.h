#ifndef TRACE_CLOCK_H_
#define TRACE_CLOCK_H_

#include <memory>

#include "clock.h"
namespace coding_nerd::boot_perf {
class TraceClock {
  using type = TraceClock;

 public:
  TraceClock() = default;
  virtual ~TraceClock() = default;
  TraceClock(TraceClock& other) = delete;
  TraceClock& operator=(TraceClock& other) = delete;
  TraceClock(TraceClock&& other) = delete;

  void SetTraceBootCycle(uint32_t cycle);
  void SetCyclePerSec(uint32_t cyclePerSec);

  [[nodiscard]] std::chrono::nanoseconds NanoSinceBootFromCycle(
      uint32_t cycle) const;

  [[nodiscard]] std::chrono::microseconds MicroSinceBootFromCycle(
      uint32_t cycle) const;
  [[nodiscard]] const uint32_t& GetCycleTurnOver() const;
  void TurnOverCycle();

  [[nodiscard]] bool IsValid() const;

 private:
  uint32_t trace_boot_cycle_ = 0;
  uint32_t trace_cycle_turn_over_ = 0;
  uint32_t cycle_per_sec_ = 0;
};
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_CLOCK_H_
