#ifndef STREAM_DUMPER_H_
#define STREAM_DUMPER_H_

#include <algorithm>
#include <ios>
#include <iterator>

#include "dumper.h"
#include "runnable.h"

namespace coding_nerd::boot_perf {
template <typename Out>
class StreamDumper : public Dumper<ProcessStatus, Out>, public Runnable {
  using type = StreamDumper<ProcessStatus, Out>;

 public:
  StreamDumper(const In&& in, Out&& out) : in_(in), out_(out){};
  ~StreamDumper() override = default;
  virtual void dump() {
    in_.seekg(0, std::ios::beg);
    out_.seekg(0, std::ios::beg);
    std::copy(std::istreambuf_iterator<char>(in_),
              std::istreambuf_iterator<char>(),
              std::ostreambuf_iterator<char>(out_));
  };
  void run() override { dump(); };

 private:
  const In in_;
  Out out_;
};
}  // namespace coding_nerd::boot_perf
#endif  // STREAM_DUMPER_H_
