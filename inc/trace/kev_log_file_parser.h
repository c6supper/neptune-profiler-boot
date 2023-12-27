#ifndef KEV_LOG_PARSER_H_
#define KEV_LOG_PARSER_H_

#include <fstream>
#include <memory>

#include "../logger.h"
#include "event_factory.h"
#include "trace_header.h"
#include "trace_parser.h"

namespace coding_nerd::boot_perf {
template <typename Out>
class KeyLogFileParser : public TraceParser<std::ifstream, Out> {
  using type = KeyLogFileParser<Out>;

 public:
  KeyLogFileParser() = default;
  ~KeyLogFileParser() override = default;
  KeyLogFileParser(KeyLogFileParser& other) = delete;
  KeyLogFileParser& operator=(KeyLogFileParser& other) = delete;

  void Parse(std::ifstream& ifs) override {
    trace_header_ = std::make_unique<TraceHeader>(ifs);
    traceevent event;
    static uint32_t timestamp = 0;
    while (!ifs.eof() || !ifs.fail() || !ifs.bad()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      traceevent last_event;
      ifs.read(reinterpret_cast<char*>(&event), sizeof(event));
      if (timestamp == 0) {
        timestamp = event.data[0];
      }
      printf("t:%8uns CPU:%02d 0x%x:0x%x",
             (event.data[0] - timestamp) *
                 (1000 * 1000 * 1000 / trace_header_->CyclesPerSec()),
             _NTO_TRACE_GETCPU(event.header), event.data[1], event.data[2]);
      last_event = event;
      switch (_TRACE_GET_STRUCT(event.header)) {
        case _TRACE_STRUCT_CC:
          printf("_TRACE_STRUCT_CC\n");
          break;
        case _TRACE_STRUCT_CB:
          printf("_TRACE_STRUCT_CB\n");
          break;
        case _TRACE_STRUCT_S:
          printf("_TRACE_STRUCT_S\n");
          break;
        case _TRACE_STRUCT_CE:
          printf("_TRACE_STRUCT_CE\n");
          break;
        default:
          break;
      }
    }
  }

 private:
  std::unique_ptr<TraceHeader> trace_header_;
};
}  // namespace coding_nerd::boot_perf

#endif  // KEV_LOG_PARSER_H_
