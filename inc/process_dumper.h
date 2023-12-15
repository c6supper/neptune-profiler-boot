#ifndef PROCESS_DUMPER_H_
#define PROCESS_DUMPER_H_

#include <string>

namespace coding_nerd::boot_perf {
struct ProcessStatus {
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

  friend std::ostream& operator<<(std::ostream& os, ProcessStatus const& ps) {
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

  friend std::ostream& operator<<(std::ostream& out, const dHeader& h) {
    return out << h.blockID << " " << h.blockLen << " " << h.bodyNum;
  }
  inline Result<ProcessStatus> status(pid_t pid) {
    std::string path = "/proc/" + std::to_string(pid) + "/stat";

    std::string _;  // For ignoring fields.

    // Parse all fields from stat.
    ProcessStatus ps;
    if (file >> _ >> ps.comm >> ps.state >> ps.ppid >> ps.pgrp >> ps.session >>
        ps.tty_nr >> ps.tpgid >> ps.flags >> ps.minflt >> ps.cminflt >>
        ps.majflt >> ps.cmajflt >> ps.utime >> ps.stime >> ps.cutime >>
        ps.cstime >> ps.priority >> ps.nice >> ps.num_threads >>
        ps.itrealvalue >> ps.starttime >> ps.vsize >> ps.rss >> ps.rsslim >>
        ps.startcode >> ps.endcode >> ps.startstack >> ps.kstkeip >>
        ps.signal >> ps.blocked >> ps.sigcatch >> ps.wchan >> ps.nswap >>
        ps.cnswap) {
      return ps;
    } else {
      return Error("Failed to read/parse '" + path + "'");
    }
  }
};

template <typename Out>
class ProcessDumper : public Dumper<ProcessStatus, Out>, public Runnable {
  using type = ProcessDumper<ProcessStatus, Out>;

 public:
  ProcessDumper(const ProcessStatus&& in, Out&& out)
      : in_(std::move(in)), out_(out){};
  ~ProcessDumper() override = default;
  virtual void dump() {
    std::string path = "/proc/" + std::to_string(pid) + "/stat";

    std::string _;  // For ignoring fields.

    // Parse all fields from stat.
    ProcessStatus ps;
    if (file >> _ >> ps.comm >> ps.state >> ps.ppid >> ps.pgrp >> ps.session >>
        ps.tty_nr >> ps.tpgid >> ps.flags >> ps.minflt >> ps.cminflt >>
        ps.majflt >> ps.cmajflt >> ps.utime >> ps.stime >> ps.cutime >>
        ps.cstime >> ps.priority >> ps.nice >> ps.num_threads >>
        ps.itrealvalue >> ps.starttime >> ps.vsize >> ps.rss >> ps.rsslim >>
        ps.startcode >> ps.endcode >> ps.startstack >> ps.kstkeip >>
        ps.signal >> ps.blocked >> ps.sigcatch >> ps.wchan >> ps.nswap >>
        ps.cnswap) {
      return ps;
    } else {
      return Error("Failed to read/parse '" + path + "'");
    }
  };
  void run() override { dump(); };

 private:
  const ProcessStatus in_;
  Out out_;
};
}  // namespace coding_nerd::boot_perf

#endif  // PROCESS_DUMPER_H_
