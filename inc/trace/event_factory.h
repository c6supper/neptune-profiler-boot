#ifndef EVENT_FACTORY_H_
#define EVENT_FACTORY_H_

#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "trace/trace_type.h"

namespace coding_nerd::boot_perf {
class EventFactory {
  using type = EventFactory;

 public:
  EventFactory() = default;
  virtual ~EventFactory() = default;

  static std::shared_ptr<EventFactory> Get();

 private:
  std::mutex mapMutex_;
  std::map<uint32_t, std::vector<std::shared_ptr<const traceevent>>>
      combinedEvent_;
};
}  // namespace coding_nerd::boot_perf

#endif  // EVENT_FACTORY_H_
