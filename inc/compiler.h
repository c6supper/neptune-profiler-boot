#ifndef COMPILER_H_
#define COMPILER_H_

#define CODING_NERD_BOOT_PERF(x) CODING_NERD_BOOT_PERF_PRIVATE_##x()

#if defined(__clang__)
#define CODING_NERD_BOOT_PERF_PRIVATE_IGNORE_PADDED_PUSH() \
  _Pragma("clang diagnostic push")                         \
      _Pragma("clang diagnostic ignored \"-Wpadded\"")
#define CODING_NERD_BOOT_PERF_PRIVATE_IGNORE_PADDED_POP() \
  _Pragma("clang diagnostic pop")
#else
#define CODING_NERD_BOOT_PERF_PRIVATE_IGNORE_PADDED_PUSH()
#define CODING_NERD_BOOT_PERF_PRIVATE_IGNORE_PADDED_POP()
#endif

#if defined(__GNUC__)
#define CODING_NERD_BOOT_PERF_PRIVATE_IGNORE_EFFCPP_PUSH() \
  _Pragma("GCC diagnostic push") _Pragma(                  \
      "GCC diagnostic ignored "                            \
      "\"-Weffc++\"")
#define CODING_NERD_BOOT_PERF_PRIVATE_IGNORE_EFFCPP_POP() \
  _Pragma("GCC diagnostic pop")
#else
#define CODING_NERD_BOOT_PERF_PRIVATE_IGNORE_EFFCPP_PUSH()
#define CODING_NERD_BOOT_PERF_PRIVATE_IGNORE_EFFCPP_POP()
#endif

#if defined(_MSC_VER)
#define CODING_NERD_BOOT_PERF_PRIVATE_NOINLINE() __declspec(noinline)
#else
#define CODING_NERD_BOOT_PERF_PRIVATE_NOINLINE() __attribute__((noinline))
#endif

// workaround missing "is_trivially_copyable" in g++ < 5.0
// See https://stackoverflow.com/a/31798726/48181
#if defined(__GNUC__) && __GNUC__ < 5
#define CODING_NERD_BOOT_PERF_IS_TRIVIALLY_COPYABLE(...) \
  __has_trivial_copy(__VA_ARGS__)
#else
#define CODING_NERD_BOOT_PERF_IS_TRIVIALLY_COPYABLE(...) \
  std::is_trivially_copyable<__VA_ARGS__>::value
#endif

#ifdef _MSC_VER
#define CODING_NERD_PACK(...) \
  __pragma(pack(push, 1)) __VA_ARGS__ __pragma(pack(pop))
#elif defined __GNUC__
#define CODING_NERD_PACK(...) __VA_ARGS__ __attribute__((__packed__))
#endif

// noexcept may be missing for std::string.
// See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58265
#define CODING_NERD_BOOT_PERF_PRIVATE_NOEXCEPT_STRING_MOVE() \
  std::is_nothrow_move_assignable<std::string>::value

#endif  // COMPILER_H_
