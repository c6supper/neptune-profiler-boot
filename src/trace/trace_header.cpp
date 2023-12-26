#include "trace/trace_header.h"

#include <iostream>
#include <memory>
#include <regex>

#include "trace/trace_type.h"

namespace coding_nerd::boot_perf {

std::vector<std::string> TraceHeader::keys_ = {_TRACE_HEADER_KEYWORDS()};

TraceHeader::TraceHeader(const std::string& header_str) {
  for (const auto& key : keys_) {
    const std::regex rgx(std::move(key + "(.*?)TRACE_"));
    std::smatch match;

    if (std::regex_search(header_str.begin(), header_str.end(), match, rgx)) {
      attributes_map_.insert(
          std::make_pair(std::move(key), std::move(match[1])));
    }
  }
  for (auto& attr : attributes_map_) {
    std::cout << ' ' << attr.first << attr.second << '\n';
  }
}

}  // namespace coding_nerd::boot_perf