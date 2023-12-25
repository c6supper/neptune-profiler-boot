#ifndef TRACE_PARSER_H_
#define TRACE_PARSER_H_

namespace coding_nerd::boot_perf {
template <typename In, typename Out>
class TraceParser {
  using type = TraceParser<In, Out>;

 public:
  TraceParser() = default;
  virtual ~TraceParser() = default;
  virtual void parse() = 0;
};
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_PARSER_H_
