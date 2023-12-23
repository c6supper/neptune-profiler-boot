#ifndef STAT_DUMPER_H_
#define STAT_DUMPER_H_

#include <dirent.h>
#include <sys/types.h>

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "clock.h"
#include "dumper.h"
#include "runnable.h"

namespace coding_nerd::boot_perf {

struct ProcStat {
  template <typename T>
  struct Sequence {
    T total;
    std::vector<T> per_item;
  };

  struct CPU {
    uint64_t user{0};
    uint64_t nice{0};
    uint64_t system{0};
    uint64_t idle{0};
    uint64_t io_wait{0};
    uint64_t irq{0};
    uint64_t softirq{0};
    uint64_t steal{0};
    uint64_t guest{0};
    uint64_t guest_nice{0};
  };

  Sequence<CPU> cpus;
  Sequence<uint64_t> intr;
  uint64_t ctxt;
  uint64_t btime;
  uint64_t processes;
  size_t procs_running;
  size_t procs_blocked;
  Sequence<uint64_t> softirq;
};

template <typename Out>
class StatDumper : public Dumper<void, Out>, public Runnable {
  using type = StatDumper<Out>;

 public:
  explicit StatDumper(Out&& out) : out_(std::move(out)){};
  ~StatDumper() override = default;
  void dump() override {
    out_ << BootPerfClock<void>::get_uptime_jiffies() << "\n";

    std::ifstream const ifstat(std::move("/proc/stat"));
    out_ << ifstat.rdbuf();

    out_ << "\n";
  }

  void run() override { dump(); };

 private:
  Out out_;
};

}  // namespace coding_nerd::boot_perf

#endif  // STAT_DUMPER_H_
