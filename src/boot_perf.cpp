#include "boot_perf.h"

#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <thread>

#include "runtime.h"
#include "trace/event_factory.h"
#include "trace/trace_header.h"
#include "trace/trace_type.h"

sigset_t make_sigset(std::initializer_list<int32_t> signals) {
  sigset_t set;
  const int32_t result = sigemptyset(&set);

  std::for_each(signals.begin(), signals.end(),
                [&set](int32_t signal) { sigaddset(&set, signal); });

  return set;
}

int main(const int argc, const char* argv[]) {
  if (!RuntimeContext().CreateArg(argc, argv)) {
    exit(0);
  }

#if 0
  BootPerfContext().Start<>(std::chrono::milliseconds(200));

  const auto signal_list = make_sigset({SIGTERM, SIGSEGV, SIGINT, SIGABRT});

  pthread_sigmask(SIG_BLOCK, &signal_list, nullptr);

  int32_t last_signal;
  do {
    sigwait(&signal_list, &last_signal);
    std::cout << "Got signal " << last_signal << std::endl;

    // Exit on sigint so ctrl+c still works
  } while (last_signal != SIGINT);

  BootPerfContext().Stop();

  coding_nerd::boot_perf::EventFactory::get();

#else

  try {
    std::ifstream ifs(std::move(Input()), std::ios::binary);

    char buff[2048];
    ifs.read(buff, sizeof(buff));
    std::string const str(buff);

    std::size_t const pos =
        str.find("TRACE_HEADER_END::") + std::strlen("TRACE_HEADER_END::");

    const coding_nerd::boot_perf::TraceHeader test(str);

    if (Verbose()) {
      std::cout << "string found at position: " << static_cast<int>(pos)
                << "\n";
    }
    ifs.seekg(pos + 51984);
    // ifs.seekg(52546);

    std::cout << ifs.tellg() << "\n";

    traceevent event;
    while (!ifs.eof() || !ifs.fail() || !ifs.bad()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      traceevent last_event;
      ifs.read(reinterpret_cast<char*>(&event), sizeof(event));
      if (std ::memcmp(&last_event, &event, sizeof(last_event)) == 0) {
        continue;
      }
      printf("t:0x%08x CPU:%02d 0x%x:0x%x", event.data[0],
             _NTO_TRACE_GETCPU(event.header), event.data[1], event.data[2]);
      last_event = event;
      switch (_TRACE_GET_STRUCT(event.header)) {
        case _TRACE_STRUCT_CC:
          printf("_TRACE_STRUCT_CC\n");
          break;
        case _TRACE_STRUCT_CB:
          printf("_TRACE_STRUCT_CB\n");
          break;
        case _TRACE_STRUCT_S:
          printf("_TRACE_STRUCT_S\n");
          break;
        case _TRACE_STRUCT_CE:
          printf("_TRACE_STRUCT_CE\n");
          break;
        default:
          break;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

#endif
}
