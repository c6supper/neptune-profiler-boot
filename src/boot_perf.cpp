#include <cstdint>
#include <iostream>
#ifdef __linux__
#include "compatible/linux_compatible.h"
#endif
#include "unused.h"

int main(int argc, char* argv[]) {
  MONUnusedParameter(argc);
  MONUnusedParameter(argv);
  std::cout << sizeof(uint32_t) << '\n';
}