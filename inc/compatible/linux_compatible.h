#ifndef LINUX_COMPATIBLE_H_
#define LINUX_COMPATIBLE_H_

#include <pthread.h>

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

struct DebugThreadInfo64 {
  pid_t pid;
  pthread_t tid;
  uint32_t flags;
  uint16_t why;
  uint16_t what;
  uint64_t ip;
  uint64_t sp;
  uint64_t stkbase;
  uint64_t tls;
  uint32_t stksize;
  uint32_t tid_flags;
  uint8_t priority;
  uint8_t real_priority;
  uint8_t policy;
  uint8_t state;
  int16_t syscall;
  uint16_t last_cpu;
  uint32_t timeout;
  int32_t last_chid;
  sigset_t sig_blocked;
  sigset_t sig_pending;
  __siginfo32_t _info32;

  union {
    struct {
      pthread_t tid;
    } join;
    struct {
      intptr64_t id;
      uintptr64_t sync;
    } sync;
    struct {
      uint32_t nd;
      pid_t pid;
      int32_t coid;
      int32_t chid;
      int32_t scoid;
      uint32_t flags;
    } connect;
    struct {
      int32_t chid;
    } channel;
    struct {
      pid_t pid;
      uint32_t flags;
      uintptr64_t vaddr;
    } waitpage;
    struct {
      size64_t size;
    } stack;
    struct {
      pthread_t tid;
    } thread_event;
    struct {
      pid_t child;
    } fork_event;
    uint64_t filler[4];
  } blocked;

  uint64_t start_time;
  uint64_t sutime;
  uint8_t extsched[8];
  uint64_t nsec_since_block;

  union {
    __siginfo32_t info32;
    __siginfo64_t info64;
    siginfo_t info;
  };

  uint64_t reserved2[4];
};

#endif  // LINUX_COMPATIBLE_H_
