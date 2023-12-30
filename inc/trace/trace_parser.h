#ifndef TRACE_PARSER_H_
#define TRACE_PARSER_H_

#include <thread>
#include <utility>

#include "logger.h"

namespace coding_nerd::boot_perf {
template <typename In, typename Out>
class TraceParser {
  using type = TraceParser<In, Out>;

 public:
  TraceParser() = default;
  virtual ~TraceParser() = default;
  virtual void Parse(In&) = 0;
  void Start(In& in) {
    running_ = true;
    parser_ = std::thread([&] { Parse(in); });
  }
  void Stop() {
    running_ = false;
    InfoLogger() << "Wait for current loop to finish.";
    parser_.join();
  }

  [[nodiscard]] bool IsRunning() const { return running_; };

 protected:
  bool running_ = false;

 private:
  std::thread parser_;
};
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_PARSER_H_
