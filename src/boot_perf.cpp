
#include "boot_perf.h"

#include <cstdint>
#include <iostream>

#include "unused.h"

int main(int argc, char* argv[]) {
  MONUnusedParameter(argc);
  MONUnusedParameter(argv);
  std::cout << sizeof(uint32_t) << '\n';
}