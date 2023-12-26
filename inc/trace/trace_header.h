#ifndef TRACE_HEADER_H_
#define TRACE_HEADER_H_

#include <mutex>
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

  const std::string& attribute(const std::string& key);

 private:
  std::unordered_map<std::string, std::string> attributes_map_;
  std::mutex attributes_mutex_;
  static std::vector<std::string> keys_;
  static std::string empty_;
};
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_HEADER_H_
