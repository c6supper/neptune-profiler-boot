
#include "boot_perf.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include "arg.hpp"
#include "unused.h"

int main(const int argc, const char* argv[]) {
  MONUnusedParameter(argc);
  MONUnusedParameter(argv);

  auto options = parse(argc, argv);

  if (!options) {
    exit(0);
  }
}