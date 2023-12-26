#ifndef TRACE_HEADER_H_
#define TRACE_HEADER_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace coding_nerd::boot_perf {
class TraceHeader {
  using type = TraceHeader;

 public:
  TraceHeader() = delete;
  explicit TraceHeader(const std::string& header_str);
  ~TraceHeader() = default;
  TraceHeader(TraceHeader& other) = delete;
  TraceHeader& operator=(TraceHeader& other) = delete;

 private:
  std::unordered_map<std::string, std::string> attributes_map_;
  static std::vector<std::string> keys_;
};
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_HEADER_H_
