#ifndef RUNTIME_H_
#define RUNTIME_H_

#include <memory>

#include "cxxopts.hpp"

#define RuntimeContext() coding_nerd::boot_perf::Runtime::Instance()

namespace coding_nerd::boot_perf {

class Runtime {
 public:
  Runtime() = default;
  ~Runtime() = default;
  Runtime(Runtime& other) = delete;
  Runtime& operator=(Runtime& other) = delete;
  static Runtime& Instance();
  bool create_option(int argc, const char* argv[]);
  std::shared_ptr<cxxopts::Options> get_option();

 private:
  std::shared_ptr<cxxopts::Options> options_;
};
}  // namespace coding_nerd::boot_perf

#endif  // RUNTIME_H_
