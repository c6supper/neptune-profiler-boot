#ifndef PROCESS_DUMPER_H_
#define PROCESS_DUMPER_H_

#include <dirent.h>
#include <sys/types.h>

#include <memory>
#include <sstream>
#include <string>

#include "clock.h"
#include "dumper.h"
#include "runnable.h"

namespace coding_nerd::boot_perf {
struct ProcessStat {
  pid_t pid;
  std::string comm;
  char state;
  pid_t ppid, pgrp, session;
  int tty_nr;
  pid_t tpgid;
  unsigned int flags;
  uint64_t minflt, cminflt, majflt, cmajflt;
  uint64_t utime, stime;
  int64_t cutime, cstime, priority, nice, num_threads, itrealvalue;
  uint64_t starttime;
  uint64_t vsize;
  int64_t rss;
  uint64_t rsslim, startcode, endcode, startstack, kstkeip, signal, blocked,
      sigcatch, wchan, nswap, cnswap;

  friend std::ostream& operator<<(std::ostream& os, ProcessStat const& ps) {
    return os << "pid: " << ps.pid << "\n"
              << "comm: " << ps.comm << "\n"
              << "state: " << ps.state << "\n"
              << "ppid: " << ps.ppid << "\n"
              << "pgrp: " << ps.pgrp << "\n"
              << "session: " << ps.session << "\n"
              << "tty_nr: " << ps.tty_nr << "\n"
              << "tpgid: " << ps.tpgid << "\n"
              << "flags: " << ps.flags << "\n"
              << "minflt: " << ps.minflt << "\n"
              << "cminflt: " << ps.cminflt << "\n"
              << "majflt: " << ps.majflt << "\n"
              << "cmajflt: " << ps.cmajflt << "\n"
              << "utime: " << ps.utime << "\n"
              << "stime: " << ps.stime << "\n"
              << "cutime: " << ps.cutime << "\n"
              << "cstime: " << ps.cstime << "\n"
              << "priority: " << ps.priority << "\n"
              << "nice: " << ps.nice << "\n"
              << "num_threads: " << ps.num_threads << "\n"
              << "itrealvalue: " << ps.itrealvalue << "\n"
              << "starttime: " << ps.starttime << "\n"
              << "vsize: " << ps.vsize << "\n"
              << "rss: " << ps.rss << "\n"
              << "rsslim: " << ps.rsslim << "\n"
              << "startcode: " << ps.startcode << "\n"
              << "endcode: " << ps.endcode << "\n"
              << "startstack: " << ps.startstack << "\n"
              << "kstkeip: " << ps.kstkeip << "\n"
              << "signal: " << ps.signal << "\n"
              << "blocked: " << ps.blocked << "\n"
              << "sigcatch: " << ps.sigcatch << "\n"
              << "wchan: " << ps.wchan << "\n"
              << "nswap: " << ps.nswap << "\n"
              << "cnswap: " << ps.cnswap << "\n";
  }

  // Result<ProcessStat> stat(pid_t pid) {
  //   std::string path = "/proc/" + std::to_string(pid) + "/stat";

  //   std::string _;  // For ignoring fields.

  //   // Parse all fields from stat.
  //   ProcessStat const ps;
  //   if (file >> _ >> ps.comm >> ps.state >> ps.ppid >> ps.pgrp >> ps.session
  //   >>
  //       ps.tty_nr >> ps.tpgid >> ps.flags >> ps.minflt >> ps.cminflt >>
  //       ps.majflt >> ps.cmajflt >> ps.utime >> ps.stime >> ps.cutime >>
  //       ps.cstime >> ps.priority >> ps.nice >> ps.num_threads >>
  //       ps.itrealvalue >> ps.starttime >> ps.vsize >> ps.rss >> ps.rsslim >>
  //       ps.startcode >> ps.endcode >> ps.startstack >> ps.kstkeip >>
  //       ps.signal >> ps.blocked >> ps.sigcatch >> ps.wchan >> ps.nswap >>
  //       ps.cnswap) {
  //     return ps;
  //   }
  //   return Error("Failed to read/parse '" + path + "'");
  // }
};

template <typename Out>
class ProcessDumper : public Dumper<void, Out>, public Runnable {
  using type = ProcessDumper<Out>;

 public:
  explicit ProcessDumper(Out&& out) : out_(std::move(out)){};
  ~ProcessDumper() override = default;
  void dump() override {
    out_ << BootPerfClock<void>::get_uptime_jiffies() << "\n";
    const std::unique_ptr<DIR, int (*)(DIR*)> dir(opendir("/proc"), closedir);
    struct dirent* entry;
    while ((entry = readdir(dir.get())) != nullptr) {
      // Only match numeric values.
      const int pid = std::atoi(entry->d_name);
      if (pid == 0) continue;

      std::stringstream ss;
      ss << "/proc/" << pid << "/cmdline";

      // /proc/<pid>/stat only has truncated task names, so get the full
      // name from /proc/<pid>/cmdline.
      std::string cmdline;
      std::ifstream if_name(ss.str());

      std::getline(if_name, cmdline);

      std::stringstream().swap(ss);
      ss << "/proc/" << pid << "/stat";

      // Read process stat line.
      std::string stat;

      std::ifstream ifstat(ss.str());
      std::getline(ifstat, stat);
      if (!stat.empty()) {
        if (!cmdline.empty()) {
          // Substitute the process name with its real name.
          const size_t open = stat.find('(');
          const size_t close = stat.find_last_of(')');
          if (open != std::string::npos && close != std::string::npos) {
            stat.replace(open + 1, close - open - 1, cmdline.c_str());
          }
        }
        out_ << stat;
      }
    }

    out_ << "\n";
  };
  void run() override { dump(); };

 private:
  Out out_;
};
}  // namespace coding_nerd::boot_perf

#endif  // PROCESS_DUMPER_H_
