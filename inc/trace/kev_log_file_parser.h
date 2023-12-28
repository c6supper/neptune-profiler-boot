#ifndef KEV_LOG_PARSER_H_
#define KEV_LOG_PARSER_H_

#include <fstream>
#include <map>
#include <memory>
#include <vector>

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
        trace_clock_(std::make_shared<TraceClock>()),
        single_event_json_factory_(
            EventFactory<nlohmann::json, traceevent>::Instance()){};
  ~KeyLogFileParser() override = default;
  KeyLogFileParser(KeyLogFileParser& other) = delete;
  KeyLogFileParser& operator=(KeyLogFileParser& other) = delete;

  void Parse(std::ifstream& ifs) override {
    trace_header_ = std::make_unique<TraceHeader>(ifs);
    trace_clock_->SetCyclePerSec(trace_header_->CyclesPerSec());

    while (!ifs.eof() || !ifs.fail() || !ifs.bad()) {
      auto event = std::make_shared<traceevent>();
      ifs.read(reinterpret_cast<char*>(event.get()), sizeof(traceevent));

      trace_clock_->SetTraceBootCycle(event->data[0]);

      if (Verbose()) {
        TraceEvent<traceevent>::Dump(*event, *trace_clock_);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      nlohmann::json json;
      const uint32_t timestamp = event->data[0];
      switch (_TRACE_GET_STRUCT(event->header)) {
        case _TRACE_STRUCT_CC:
        case _TRACE_STRUCT_CB:
          composing_lock_.lock();
          composing_event_map_[timestamp].push_back(std::move(*event));
          composing_lock_.unlock();
          break;
        case _TRACE_STRUCT_S:
          single_event_json_factory_->Convert(json, *event, *trace_clock_);
          break;
        case _TRACE_STRUCT_CE:
          composing_lock_.lock();

          composing_event_map_[timestamp].push_back(std::move(*event));
          composing_event_json_factory_->Convert(
              json, composing_event_map_[timestamp], *trace_clock_);
          composing_event_map_.erase(timestamp);
          composing_lock_.unlock();
          break;
        default:
          break;
      }

      if (!json.empty()) {
        VerboseLogger() << json;
      }
    }
  }

 private:
  std::unique_ptr<TraceHeader> trace_header_;
  std::shared_ptr<TraceClock> trace_clock_;
  std::shared_ptr<EventFactory<nlohmann::json, traceevent>>
      single_event_json_factory_;
  std::shared_ptr<EventFactory<nlohmann::json, std::vector<traceevent>>>
      composing_event_json_factory_;
  std::map<uint32_t, std::vector<traceevent>> composing_event_map_;
  std::mutex composing_lock_;
};

}  // namespace coding_nerd::boot_perf

#endif  // KEV_LOG_PARSER_H_
