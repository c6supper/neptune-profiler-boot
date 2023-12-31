#ifndef KEV_LOG_PARSER_H_
#define KEV_LOG_PARSER_H_

#include <cstdint>
#include <fstream>
#include <future>
#include <ios>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include "event_factory.h"
#include "json.hpp"
#include "trace/trace_clock.h"
#include "trace/trace_type.h"
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
        event_json_factory_(
            EventFactory<nlohmann::json, traceevent>::Instance()),
        event_ftrace_factory_(
            EventFactory<std::string, traceevent>::Instance()){};
  ~KeyLogFileParser() override = default;
  KeyLogFileParser(KeyLogFileParser& other) = delete;
  KeyLogFileParser& operator=(KeyLogFileParser& other) = delete;

  void Parse(std::ifstream& ifs) override {
    trace_header_ = std::make_unique<TraceHeader>(ifs);
    trace_clock_->SetCyclePerSec(trace_header_->CyclesPerSec());

    std::vector<std::future<void>> futures;
    const std::string output = std::move(Output());
    std::ofstream ofs(
        output, std::ios::binary | std::ios_base::out | std::ios_base::trunc);
    ofs << R"({"traceEvents": [)";
    while ((!ifs.eof() || !ifs.fail() || !ifs.bad()) && this->IsRunning()) {
      auto event = std::make_shared<traceevent>();
      ifs.read(reinterpret_cast<char*>(event.get()), sizeof(traceevent));

      trace_clock_->SetTraceBootCycle(event->data[0]);

      auto async_parse = [&, event]() {
        nlohmann::json json;
        if (Verbose()) {
          TraceEvent<traceevent>::Dump(*event, *trace_clock_);
          // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        const uint32_t timestamp = event->data[0];
        std::vector<traceevent> ev;
        switch (_TRACE_GET_STRUCT(event->header)) {
          case _TRACE_STRUCT_CC:
          case _TRACE_STRUCT_CB:
            composing_lock_.lock();
            composing_event_map_[timestamp].push_back(std::move(*event));
            composing_lock_.unlock();
            break;
          case _TRACE_STRUCT_S:
            ev.push_back(std::move(*event));
            event_json_factory_->Convert(json, ev, *trace_clock_);
            break;
          case _TRACE_STRUCT_CE:
            composing_lock_.lock();

            composing_event_map_[timestamp].push_back(std::move(*event));
            event_json_factory_->Convert(json, composing_event_map_[timestamp],
                                         *trace_clock_);
            composing_event_map_.erase(timestamp);
            composing_lock_.unlock();
            break;
          default:
            break;
        }
        if (!json.empty()) {
          ofs << json << ",";
        }
      };
      auto sync_parse = [&, event]() {
        std::string ftrace;
        if (Verbose()) {
          TraceEvent<traceevent>::Dump(*event, *trace_clock_);
          // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        const uint32_t timestamp = event->data[0];
        std::vector<traceevent> ev;
        switch (_TRACE_GET_STRUCT(event->header)) {
          case _TRACE_STRUCT_CC:
          case _TRACE_STRUCT_CB:
            composing_lock_.lock();
            composing_event_map_[timestamp].push_back(std::move(*event));
            composing_lock_.unlock();
            break;
          case _TRACE_STRUCT_S:
            ev.push_back(std::move(*event));
            event_ftrace_factory_->Convert(ftrace, ev, *trace_clock_);
            break;
          case _TRACE_STRUCT_CE:
            composing_lock_.lock();
            composing_event_map_[timestamp].push_back(std::move(*event));
            event_ftrace_factory_->Convert(
                ftrace, composing_event_map_[timestamp], *trace_clock_);
            composing_event_map_.erase(timestamp);
            composing_lock_.unlock();
            break;
          default:
            break;
        }
        if (!ftrace.empty()) {
          ofs << ftrace;
        }
      };
      if (Ftrace()) {
        futures.push_back(
            std::move(std::async(std::launch::async, sync_parse)));
      } else {
        futures.push_back(
            std::move(std::async(std::launch::async, async_parse)));
      }
      if (static_cast<uint32_t>(futures.size()) >
          (Ftrace() ? 0 : (std::thread::hardware_concurrency() * 2))) {
        for (auto& f : futures) {
          f.wait();
        }
        futures.clear();
      }
    }
    if (!Ftrace()) {
      ofs << R"(],"displayTimeUnit": "ns"})";
    } else {
      ofs << R"(,"controllerTraceDataKey": "systraceController"})";
    }
  };

 private:
  std::unique_ptr<TraceHeader> trace_header_;
  std::shared_ptr<TraceClock> trace_clock_;
  std::shared_ptr<EventFactory<nlohmann::json, traceevent>> event_json_factory_;
  std::shared_ptr<EventFactory<std::string, traceevent>> event_ftrace_factory_;
  // std::shared_ptr<EventFactory<nlohmann::json, std::vector<traceevent>>>
  //     event_json_factory_;
  std::map<uint32_t, std::vector<traceevent>> composing_event_map_;
  std::mutex composing_lock_;
  std::mutex json_lock_;
};

}  // namespace coding_nerd::boot_perf

#endif  // KEV_LOG_PARSER_H_
