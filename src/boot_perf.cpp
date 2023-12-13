
#include "boot_perf.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include "cxxopts.hpp"
#include "unused.h"

int main(int argc, char* argv[]) {
  MONUnusedParameter(argc);
  MONUnusedParameter(argv);

  coding_nerd::boot_perf::BootPerf::Instance();

  try {
    cxxopts::Options options(argv[0], "A brief description");

    options.add_options()("b,bar", "Param bar", cxxopts::value<std::string>())(
        "d,debug", "Enable debugging",
        cxxopts::value<bool>()->default_value("false"))(
        "f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))(
        "h,help", "Print usage");

    auto result = options.parse(argc, argv);

    if (result.count("help") != 0U) {
      std::cout << options.help() << '\n';
      exit(0);
    }
  } catch (...) {
    std::cerr << "Unexpected non-exception error!\n";
  }
}