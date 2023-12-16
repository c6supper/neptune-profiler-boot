#include "boot_perf.h"

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <thread>

#include "arg.hpp"
#include "unused.h"

void signalHandler(int signum) {
  std::cout << "Interrupt signal (" << signum << ") received.\n";

  BootPerfContext().Stop();

  exit(signum);
}

int main(const int argc, const char* argv[]) {
  MONUnusedParameter(argc);
  MONUnusedParameter(argv);

  auto options = parse(argc, argv);

  if (!options) {
    exit(0);
  }

  std::signal(SIGINT, signalHandler);

  BootPerfContext().Start<>(std::chrono::milliseconds(200));

  std::this_thread::sleep_until(
      std::chrono::system_clock::now() +
      std::chrono::hours(std::numeric_limits<int>::max()));

  BootPerfContext().Stop();
}
