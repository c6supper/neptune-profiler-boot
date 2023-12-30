
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <utility>

#include "logger.h"
#include "runtime.h"
#include "trace/kev_log_file_parser.h"
// NOLINTBEGIN
#include <signal.h>

sigset_t make_sigset(std::initializer_list<int32_t> signals) {
  sigset_t set;
  const int32_t result = sigemptyset(&set);

  std::for_each(signals.begin(), signals.end(),
                [&set](int32_t signal) { sigaddset(&set, signal); });

  return set;
}
// NOLINTEND

int main(const int argc, const char* argv[]) {
  if (!RuntimeContext().CreateArg(argc, argv)) {
    exit(0);
  }

#if boot_perf
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

#else

  try {
    std::ifstream ifs(std::move(Input()), std::ios::binary);

    const auto signal_list = make_sigset({SIGINT});

    pthread_sigmask(SIG_BLOCK, &signal_list, nullptr);
    coding_nerd::boot_perf::KeyLogFileParser<std::ofstream> parser;
    parser.Start(ifs);
    int32_t last_signal;
    do {
      sigwait(&signal_list, &last_signal);
      InfoLogger() << "Got signal " << last_signal;
      parser.Stop();

      // Exit on sigint so ctrl+c still works
    } while (last_signal != SIGINT);

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

#endif
}
