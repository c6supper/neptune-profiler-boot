#ifndef PROCESS_EVENT_H_
#define PROCESS_EVENT_H_

#include "event_factory.h"
#include "trace_event.h"
#include "trace_parser.h"
namespace coding_nerd::boot_perf {

template <typename T>
struct ProcessEvent : TraceEvent<T> {
  ProcessEvent() = delete;
  ProcessEvent(ProcessEvent& other) = delete;
  ProcessEvent& operator=(ProcessEvent& other) = delete;
  ProcessEvent(ProcessEvent&& other) = delete;
  ~ProcessEvent() override = default;
  explicit ProcessEvent(std::shared_ptr<T>& event,
                        std::shared_ptr<TraceClock>& clock)
      : TraceEvent<T>(event, clock){};

  static void ToJson(nlohmann::json& j, const T& e,
                     const TraceClock& trace_clock) {
    TraceEvent<T>::ToJson(j, e, trace_clock);
  };
};
template <typename T>
DoRegisterConverter<nlohmann::json, T> process_json_converter;
}  // namespace coding_nerd::boot_perf
#endif  // PROCESS_EVENT_H_
