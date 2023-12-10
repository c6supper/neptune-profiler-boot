#ifndef LINUX_COMPATIBLE_H_
#define LINUX_COMPATIBLE_H_

#include <cstdint>

struct Clockadjust {
  uint32_t tick_count;
  int32_t tick_nsec_inc;
};

struct QtimeEntry {
  uint64_t cycles_per_sec; /* for ClockCycles */
  uint64_t volatile nsec_tod_adjust;
  uint64_t volatile nsec;
  uint32_t nsec_inc;
  uint32_t boot_time; /* UTC seconds when machine booted */

  struct Clockadjust adjust;
  uint32_t timer_rate; /* times 10^timer_scale */
  int32_t timer_scale;
  uint32_t timer_load;
  int32_t intr;
  uint32_t epoch;
  uint32_t flags;
  int32_t rr_interval_mul;
  uint32_t spare0;
  uint64_t volatile nsec_stable;
  uint32_t spare[4];
};

#endif  // LINUX_COMPATIBLE_H_
