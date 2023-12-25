#ifndef CONFIG_H_
#define CONFIG_H_

#ifdef DEBUG
#define BOOT_PERF_LOG_ENABLED
#else
#undef BOOT_PERF_LOG_ENABLED
#endif

#define BOOST_FILESYSTEM_NO_DEPRECATED

#endif  // CONFIG_H_
