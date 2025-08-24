#include <string>

#include "event_factory.h"
#include "json.hpp"
#include "process_event.h"
#include "thread_event.h"
#include "trace_type.h"

namespace {
coding_nerd::boot_perf::DoRegisterConverter<nlohmann::json, traceevent>
    process_json_converter{
        _NTO_TRACE_PROCESS,
        coding_nerd::boot_perf::ProcessEvent<traceevent>::ToJson};

coding_nerd::boot_perf::DoRegisterConverter<std::string, traceevent>
    thread_ftrace_converter{
        _NTO_TRACE_THREAD,
        coding_nerd::boot_perf::ThreadEvent<traceevent>::ToFtrace};

coding_nerd::boot_perf::DoRegisterConverter<std::string, traceevent>
    process_ftrace_converter{
        _NTO_TRACE_PROCESS,
        coding_nerd::boot_perf::ProcessEvent<traceevent>::ToFtrace};
}  // namespace