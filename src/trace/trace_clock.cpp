#include "trace/trace_clock.h"

#include <memory>

#include "logger.h"

namespace coding_nerd::boot_perf {

void TraceClock::SetTraceBootCycle(const uint32_t cycle) {
  if (trace_boot_cycle_ == 0) trace_boot_cycle_ = cycle;
}

void TraceClock::SetCyclePerSec(uint32_t cyclePerSec) {
  cycle_per_sec_ = cyclePerSec;
}

std::chrono::nanoseconds TraceClock::NanoSinceBootFromCycle(
    const uint32_t cycle) const {
  if (!IsValid()) FatalLogger() << "Clock haven't been initialized";
  return std::chrono::nanoseconds{
      (static_cast<uint64_t>(trace_cycle_turn_over_) << 32 |
       static_cast<uint64_t>(cycle - trace_boot_cycle_)) *
      1000 * 1000 * 1000 / cycle_per_sec_};
}

bool TraceClock::IsValid() const {
  return (trace_boot_cycle_ != 0 && cycle_per_sec_ != 0);
}

std::chrono::microseconds TraceClock::MicroSinceBootFromCycle(
    const uint32_t cycle) const {
  if (!IsValid()) FatalLogger() << "Clock haven't been initialized";

  return std::chrono::microseconds{
      (static_cast<uint64_t>(trace_cycle_turn_over_) << 32 |
       static_cast<uint64_t>(cycle - trace_boot_cycle_)) *
      1000 * 1000 / cycle_per_sec_};
}

std::chrono::milliseconds TraceClock::MilliSinceBootFromCycle(
    const uint32_t cycle) const {
  if (!IsValid()) FatalLogger() << "Clock haven't been initialized";

  return std::chrono::milliseconds{
      (static_cast<uint64_t>(trace_cycle_turn_over_) << 32 |
       static_cast<uint64_t>(cycle - trace_boot_cycle_)) *
      1000 / cycle_per_sec_};
}

const uint32_t& TraceClock::GetCycleTurnOver() const {
  return trace_cycle_turn_over_;
}
void TraceClock::TurnOverCycle() { trace_cycle_turn_over_++; }
}  // namespace coding_nerd::boot_perf