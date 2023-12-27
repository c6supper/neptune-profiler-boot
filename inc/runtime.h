#ifndef RUNTIME_H_
#define RUNTIME_H_

#include <memory>

#include "cxxopts.hpp"

#define RuntimeContext() coding_nerd::boot_perf::Runtime::Instance()
#define ArgContext() coding_nerd::boot_perf::Runtime::Instance().GetArg()

#define Input() (*ArgContext())["input"].as<std::string>()

namespace coding_nerd::boot_perf {

class Runtime {
 public:
  Runtime() = default;
  ~Runtime() = default;
  Runtime(Runtime& other) = delete;
  Runtime& operator=(Runtime& other) = delete;
  static Runtime& Instance();
  bool CreateArg(int argc, const char* argv[]);
  std::shared_ptr<cxxopts::ParseResult> GetArg();

 private:
  std::shared_ptr<cxxopts::ParseResult> args_;
};
}  // namespace coding_nerd::boot_perf

#endif  // RUNTIME_H_
