#include "runtime.h"

#include <iostream>

namespace coding_nerd::boot_perf {

Runtime& Runtime::Instance() {
  static Runtime instance;
  return instance;
}

// NOLINTBEGIN
bool Runtime::CreateArg(int argc, const char* argv[]) {
  cxxopts::Options options(argv[0], "");
  try {
    options.positional_help("[optional args]").show_positional_help();

    bool verbose = true;
    options.set_width(70)
        .set_tab_expansion()
        .allow_unrecognised_options()
        .add_options()("v,verbose", "Enable verbose log",
                       cxxopts::value<bool>()->default_value("true"))(
            "i,input", "Input Trace File", cxxopts::value<std::string>())(
            "h,help", "Print usage");
    options.parse_positional({"input"});
    args_ = std::make_shared<cxxopts::ParseResult>(options.parse(argc, argv));

    if (args_->count("input") <= 0) {
      goto EXIT_WITH_NULLPTR;
    }

    if (args_->count("help")) {
      goto EXIT_WITH_NULLPTR;
    }
  } catch (const cxxopts::exceptions::exception& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    goto EXIT_WITH_NULLPTR;
  }

  return true;

EXIT_WITH_NULLPTR:
  args_.reset();
  std::cout << options.help() << std::endl;
  return false;
}
// NOLINTEND

std::shared_ptr<cxxopts::ParseResult> Runtime::GetArg() {
  return std::make_shared<cxxopts::ParseResult>(*args_);
}

}  // namespace coding_nerd::boot_perf