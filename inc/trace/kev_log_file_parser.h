#ifndef KEV_LOG_PARSER_H_
#define KEV_LOG_PARSER_H_

#include <fstream>
#include <memory>

#include "../logger.h"
#include "clock.h"
#include "event_factory.h"
#include "process_event.h"
#include "trace/trace_clock.h"
#include "trace_header.h"
#include "trace_parser.h"

namespace coding_nerd::boot_perf {
template <typename Out>
class KeyLogFileParser : public TraceParser<std::ifstream, Out> {
  using type = KeyLogFileParser<Out>;

 public:
  KeyLogFileParser()
      : TraceParser<std::ifstream, Out>(),
        trace_clock_(std::make_shared<TraceClock>()){};
  ~KeyLogFileParser() override = default;
  KeyLogFileParser(KeyLogFileParser& other) = delete;
  KeyLogFileParser& operator=(KeyLogFileParser& other) = delete;

  void Parse(std::ifstream& ifs) override {
    trace_header_ = std::make_unique<TraceHeader>(ifs);
    trace_clock_->SetCyclePerSec(trace_header_->CyclesPerSec());
    auto event = std::make_shared<traceevent>();

    while (!ifs.eof() || !ifs.fail() || !ifs.bad()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      ifs.read(reinterpret_cast<char*>(event.get()), sizeof(traceevent));

      trace_clock_->SetTraceBootCycle(event->data[0]);

      if (Verbose()) {
        TraceEvent<traceevent>::Dump(*event, *trace_clock_);
      }
      // const ProcessEvent<traceevent> pe(event, trace_clock_);
      // const nlohmann::json j = pe;
      // std::cout << j << std::endl;
    }
  }

 private:
  std::unique_ptr<TraceHeader> trace_header_;
  std::shared_ptr<TraceClock> trace_clock_;
};

}  // namespace coding_nerd::boot_perf

#endif  // KEV_LOG_PARSER_H_
