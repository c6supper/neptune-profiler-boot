#ifndef TRACE_HEADER_H_
#define TRACE_HEADER_H_

#include <sys/types.h>

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <fstream>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace coding_nerd::boot_perf {
class TraceHeader {
  using AttributeType = boost::variant<uint32_t, std::string>;
  using type = TraceHeader;

 public:
  TraceHeader() = delete;
  explicit TraceHeader(std::ifstream& ifs);
  ~TraceHeader() = default;
  TraceHeader(TraceHeader& other) = delete;
  TraceHeader& operator=(TraceHeader& other) = delete;

  uint32_t CyclesPerSec() const;  // NOLINT

  boost::optional<AttributeType&> Attribute(const std::string& key);

 private:
  std::unordered_map<std::string, AttributeType> attributes_map_;
  std::mutex attributes_mutex_;
  static std::vector<std::string> keys_;
  uint32_t cycles_per_sec_;
};
}  // namespace coding_nerd::boot_perf

#endif  // TRACE_HEADER_H_
