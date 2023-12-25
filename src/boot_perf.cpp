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

#include "arg.hpp"
#include "trace/event_factory.h"
#include "trace/trace_type.h"

sigset_t make_sigset(std::initializer_list<int32_t> signals) {
  sigset_t set;
  const int32_t result = sigemptyset(&set);

  std::for_each(signals.begin(), signals.end(),
                [&set](int32_t signal) { sigaddset(&set, signal); });

  return set;
}

int main(const int argc, const char* argv[]) {
  auto options = parse(argc, argv);

  if (!options) {
    exit(0);
  }

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

  std::ifstream ifs("../log/tracelogger2_20200101103408.kev", std::ios::binary);

  char buff[2048];
  ifs.read(buff, sizeof(buff));
  std::string const str(buff);

  std::size_t const pos = str.find("TRACE_HEADER_END::");
  std::cout << "string found at position: " << static_cast<int>(pos) << "\n";
  ifs.seekg(pos + std::strlen("TRACE_HEADER_END::") + 1 + 51984);
  std::cout << ifs.tellg() << "\n";
#if 0
  traceevent event;
  while (!ifs.eof() || !ifs.fail() || !ifs.bad()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    traceevent last_event;
    ifs.read(reinterpret_cast<char*>(&event), sizeof(event));
    if (std ::memcmp(&last_event, &event, sizeof(last_event)) == 0) {
      continue;
    }
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
#endif
}
