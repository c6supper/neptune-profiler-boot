#include <string>

#include "event_factory.h"
#include "json.hpp"
#include "process_event.h"
#include "thread_event.h"
#include "trace_type.h"

namespace coding_nerd::boot_perf {
DoRegisterConverter<nlohmann::json, traceevent> process_json_converter{
    _NTO_TRACE_PROCESS, ProcessEvent<traceevent>::ToJson};

DoRegisterConverter<std::string, traceevent> thread_ftrace_converter{
    _NTO_TRACE_THREAD, ThreadEvent<traceevent>::ToFtrace};

DoRegisterConverter<std::string, traceevent> process_ftrace_converter{
    _NTO_TRACE_PROCESS, ProcessEvent<traceevent>::ToFtrace};
}  // namespace coding_nerd::boot_perf