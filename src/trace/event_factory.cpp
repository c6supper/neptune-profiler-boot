#include "trace/event_factory.h"

#include <memory>

namespace coding_nerd::boot_perf {
std::shared_ptr<EventFactory> EventFactory::Instance() {
  static std::shared_ptr<EventFactory> const kTheFactory =
      std::make_shared<EventFactory>();
  return kTheFactory;
}
}  // namespace coding_nerd::boot_perf