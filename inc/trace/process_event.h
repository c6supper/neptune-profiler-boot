#ifndef PROCESS_EVENT_H_
#define PROCESS_EVENT_H_

#include "event_factory.h"
#include "trace_event.h"
#include "trace_parser.h"
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

  static void ToJson(nlohmann::json& j, const T& e,
                     const TraceClock& trace_clock) {
    TraceEvent<T>::ToJson(j, e, trace_clock);
  };
};

static DoRegisterConverter<nlohmann::json, traceevent> process_json_converter{
    _NTO_TRACE_PROCESS, ProcessEvent<traceevent>::ToJson};

}  // namespace coding_nerd::boot_perf
#endif  // PROCESS_EVENT_H_
