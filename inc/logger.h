#ifndef LOGGER_H_
#define LOGGER_H_

#if defined(BOOT_PERF_LOG_ENABLED)
#include <iostream>
#define BOOT_PERF_LOG(x)                                                    \
  do {                                                                      \
    std::cout << __FUNCTION__ << "@" << __LINE__ << ": " << x << std::endl; \
  } while (0)
#else
#define BOOT_PERF_LOG(x) \
  do {                   \
  } while (0)
#endif

#endif  // LOGGER_H_
