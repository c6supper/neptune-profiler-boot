#ifndef KEV_LOG_PARSER_H_
#define KEV_LOG_PARSER_H_

#include <fstream>

#include "trace_parser.h"

namespace coding_nerd::boot_perf {
template <typename Out>
class KeyLogFileParser : public TraceParser<ifstream, Out> {
  using type = KeyLogFileParser<Out>;

 public:
  KeyLogFileParser() = delete;
  virtual ~KeyLogFileParser() = default;
  KeyLogFileParser(KeyLogFileParser& other) = delete;
  KeyLogFileParser& operator=(KeyLogFileParser& other) = delete;
  explicit KeyLogFileParser(const std::string& input) {}

  virtual void Parse(){

  };

 private:
};
}  // namespace coding_nerd::boot_perf

#endif  // KEV_LOG_PARSER_H_
