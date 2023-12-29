#ifndef PROCESS_EVENT_H_
#define PROCESS_EVENT_H_

#include <memory>

#include "event_factory.h"
#include "json.hpp"
#include "logger.h"
#include "trace/trace_clock.h"
#include "trace_event.h"
#include "trace_type.h"
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

  static void ToJson(nlohmann::json& j, T& e, const TraceClock& trace_clock) {
    TraceEvent<T>::ToJson(j, e, trace_clock);
  };

  static void ToJson(nlohmann::json& j, std::vector<T>& e,
                     const TraceClock& trace_clock) {
    TraceEvent<T>::ToJson(j, e, trace_clock);
    VerboseLogger() << e.size();
  };
};

static DoRegisterConverter<nlohmann::json, traceevent> process_json_converter{
    _NTO_TRACE_PROCESS, ProcessEvent<traceevent>::ToJson};

// static DoRegisterConverter<nlohmann::json, std::vector<traceevent>>
//     process_composing_json_converter{
//         _NTO_TRACE_PROCESS, ProcessEvent<std::vector<traceevent>>::ToJson};

}  // namespace coding_nerd::boot_perf
#endif  // PROCESS_EVENT_H_
