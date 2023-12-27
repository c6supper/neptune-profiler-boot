#ifndef PROCESS_EVENT_H_
#define PROCESS_EVENT_H_

#include "trace_event.h"
namespace coding_nerd::boot_perf {

template <typename T>
struct ProcessEvent : TraceEvent<T> {
  ProcessEvent() = delete;
  ProcessEvent(ProcessEvent& other) = delete;
  ProcessEvent& operator=(ProcessEvent& other) = delete;
  ProcessEvent(ProcessEvent&& other) = delete;
  ~ProcessEvent() override = default;
  explicit ProcessEvent(std::shared_ptr<T> event) : TraceEvent<T>(event){};
};

template <typename T>
void to_json(nlohmann::json& j, const ProcessEvent<T>& e) {  // NOLINT
  to_json(j, static_cast<const TraceEvent<T>&>(e));
};

}  // namespace coding_nerd::boot_perf
#endif  // PROCESS_EVENT_H_
