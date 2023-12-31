#include "runtime.h"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <memory>

#include "cxxopts.hpp"

namespace coding_nerd::boot_perf {

Runtime& Runtime::Singleton() {
  static Runtime instance;
  return instance;
}

// NOLINTBEGIN
bool Runtime::CreateArg(int argc, const char* argv[]) {
  cxxopts::Options options(argv[0], "");
  auto filter = boost::log::trivial::info;
  try {
    options.positional_help("[optional args]").show_positional_help();

    bool verbose = false;
    options.set_width(70)
        .set_tab_expansion()
        .allow_unrecognised_options()
        .add_options()("v,verbose", "Enable verbose log",
                       cxxopts::value<bool>()->default_value("false"))(
            "i,input", "Input Trace File", cxxopts::value<std::string>())(
            "o,output", "Output Trace File", cxxopts::value<std::string>())(
            "f,ftrace", "Enable output to ftrace",
            cxxopts::value<bool>()->default_value("false"))("h,help",
                                                            "Print usage");
    options.parse_positional({"input"});
    options.parse_positional({"output"});
    args_ = std::make_shared<cxxopts::ParseResult>(options.parse(argc, argv));

    if (args_->count("input") <= 0 || args_->count("output") <= 0) {
      goto EXIT_WITH_NULLPTR;
    }

    if (args_->count("help")) {
      goto EXIT_WITH_NULLPTR;
    }
  } catch (const cxxopts::exceptions::exception& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    goto EXIT_WITH_NULLPTR;
  }

  // set log filter
  if ((*args_)["verbose"].as<bool>()) {
    filter = boost::log::trivial::trace;
  }
  boost::log::core::get()->set_filter(boost::log::trivial::severity >= filter);

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