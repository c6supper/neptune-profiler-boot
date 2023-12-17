#include "boot_perf.h"

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <thread>

#include "arg.hpp"
#include "unused.h"

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
}
