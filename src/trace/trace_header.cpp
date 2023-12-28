#include "trace/trace_header.h"

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <memory>
#include <regex>

#include "logger.h"
#include "trace/trace_type.h"

namespace coding_nerd::boot_perf {

std::vector<std::string> TraceHeader::keys_ = {_TRACE_HEADER_KEYWORDS()};

TraceHeader::TraceHeader(std::ifstream& ifs) {
  char buff[2048];
  ifs.read(buff, sizeof(buff));
  std::string const header_str(buff);

  std::size_t const pos = header_str.find(_TRACE_MK_HK(HEADER_END)) +
                          std::strlen(_TRACE_MK_HK(HEADER_END));

  for (const auto& key : keys_) {
    const std::regex rgx(std::move(key + "(.*?)TRACE_"));
    std::smatch match;

    if (std::regex_search(header_str.begin(), header_str.end(), match, rgx)) {
      try {
        attributes_map_[key] =
            std::move(boost::lexical_cast<uint32_t>(match[1]));
      } catch (const boost::bad_lexical_cast& e) {
        attributes_map_[key] = std::move(match[1]);
      }
    }
  }
  boost::optional<TraceHeader::AttributeType> opt_page_len =
      attributes_map_[_TRACE_MK_HK(SYSPAGE_LEN)];
  boost::optional<TraceHeader::AttributeType> opt_cycles_per_sec =
      attributes_map_[_TRACE_MK_HK(CYCLES_PER_SEC)];

  if (!opt_page_len || !opt_cycles_per_sec) {
    FatalLogger() << "Invalid key log file, syspage_len or cycles_per_sec header "
               "missing!";
  }
  const uint32_t page_len = boost::get<uint32_t>(*opt_page_len);
  cycles_per_sec_ = boost::get<uint32_t>(*opt_cycles_per_sec);
  ifs.seekg(pos + page_len);
  VerboseLogger() << "Found trace log header, end at pos = " << (pos + page_len);

  for (auto& attr : attributes_map_) {
    VerboseLogger() << ' ' << attr.first << attr.second << '\n';
  }
}

uint32_t TraceHeader::CyclesPerSec() const { return cycles_per_sec_; }

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