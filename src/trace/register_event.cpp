#include "event_factory.h"
#include "json.hpp"
#include "process_event.h"
#include "trace_type.h"

namespace coding_nerd::boot_perf {
DoRegisterConverter<nlohmann::json, traceevent> process_json_converter{
    _NTO_TRACE_PROCESS, ProcessEvent<traceevent>::ToJson};
}