#include "trace/trace_header.h"

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <memory>
#include <regex>

#include "logger.h"
#include "trace/trace_type.h"

namespace coding_nerd::boot_perf {

std::vector<std::string> TraceHeader::keys_ = {_TRACE_HEADER_KEYWORDS()};

TraceHeader::TraceHeader(const std::string& header_str) {
  for (const auto& key : keys_) {
    const std::regex rgx(std::move(key + "(.*?)TRACE_"));
    std::smatch match;

    if (std::regex_search(header_str.begin(), header_str.end(), match, rgx)) {
      try {
        attributes_map_[key] =
            std::move(boost::lexical_cast<uint32_t>(match[1]));
      } catch (const boost::bad_lexical_cast& e) {
        Verbose() << e.what() << '\n';
        attributes_map_[key] = std::move(match[1]);
      }
    }
  }
  for (auto& attr : attributes_map_) {
    Verbose() << ' ' << attr.first << attr.second << '\n';
  }
}

boost::optional<TraceHeader::AttributeType&> TraceHeader::Attribute(
    const std::string& key) {
  const std::lock_guard<std::mutex> guard(attributes_mutex_);
  auto iter = attributes_map_.find(key);
  if (iter == attributes_map_.end()) {
    return boost::none;
  }
  return iter->second;
}

}  // namespace coding_nerd::boot_perf