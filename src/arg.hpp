#ifndef ARG_H_
#define ARG_H_

#include <iostream>
#include <memory>

#include "cxxopts.hpp"

inline std::shared_ptr<cxxopts::Options> parse(const int argc,
                                               const char* argv[]) {
  auto options = std::make_shared<cxxopts::Options>(argv[0], "");
  try {
    options->positional_help("[optional args]").show_positional_help();

    bool apple = false;

    options->set_width(70)
        .set_tab_expansion()
        .allow_unrecognised_options()
        .add_options()("a,apple,ringo", "an apple",
                       cxxopts::value<bool>(apple))("b,bob", "Bob")(
            "char", "A character", cxxopts::value<char>())(
            "t,true", "True", cxxopts::value<bool>()->default_value("true"))(
            "f, file", "File", cxxopts::value<std::vector<std::string>>(),
            "FILE")("i,input", "Input", cxxopts::value<std::string>())(
            "o,output", "Output file",
            cxxopts::value<std::string>()
                ->default_value("a.out")
                ->implicit_value("b.def"),
            "BIN")("x", "A short-only option", cxxopts::value<std::string>())(
            "positional",
            "Positional arguments: these are the arguments that are entered "
            "without an option",
            cxxopts::value<std::vector<std::string>>())(
            "long-description",
            "thisisareallylongwordthattakesupthewholelineandcannotbebrokenatasp"
            "ace")("help", "Print help")("tab-expansion", "Tab\texpansion")(
            "int", "An integer", cxxopts::value<int>(), "N")(
            "float", "A floating point number", cxxopts::value<float>())(
            "vector", "A list of doubles",
            cxxopts::value<std::vector<double>>())(
            "option_that_is_too_long_for_the_help", "A very long option")(
            "l,list", "List all parsed arguments (including default values)")(
            "range", "Use range-for to list arguments")
#ifdef CXXOPTS_USE_UNICODE
            ("unicode",
             u8"A help option with non-ascii: à. Here the size of the"
             " string should be correct")
#endif
        ;

    options->add_options("Group")("c,compile", "compile")(
        "d,drop", "drop", cxxopts::value<std::vector<std::string>>());

    options->parse_positional({"input", "output", "positional"});

    auto result = options->parse(argc, argv);

    if (result.count("help")) {
      std::cout << options->help({"", "Group"}) << std::endl;
      goto EXIT_WITH_NULLPTR;
    }

    if (result.count("list")) {
      if (result.count("range")) {
        for (const auto& kv : result) {
          std::cout << kv.key() << " = " << kv.value() << std::endl;
        }
      } else {
        std::cout << result.arguments_string() << std::endl;
      }
      goto EXIT_WITH_NULLPTR;
    }

    if (apple) {
      std::cout << "Saw option ‘a’ " << result.count("a") << " times "
                << std::endl;
    }

    if (result.count("b")) {
      std::cout << "Saw option ‘b’" << std::endl;
    }

    if (result.count("char")) {
      std::cout << "Saw a character ‘" << result["char"].as<char>() << "’"
                << std::endl;
    }

    if (result.count("f")) {
      auto& ff = result["f"].as<std::vector<std::string>>();
      std::cout << "Files" << std::endl;
      for (const auto& f : ff) {
        std::cout << f << std::endl;
      }
    }

    if (result.count("input")) {
      std::cout << "Input = " << result["input"].as<std::string>() << std::endl;
    }

    if (result.count("output")) {
      std::cout << "Output = " << result["output"].as<std::string>()
                << std::endl;
    }

    if (result.count("positional")) {
      std::cout << "Positional = {";
      auto& v = result["positional"].as<std::vector<std::string>>();
      for (const auto& s : v) {
        std::cout << s << ", ";
      }
      std::cout << "}" << std::endl;
    }

    if (result.count("int")) {
      std::cout << "int = " << result["int"].as<int>() << std::endl;
    }

    if (result.count("float")) {
      std::cout << "float = " << result["float"].as<float>() << std::endl;
    }

    if (result.count("vector")) {
      std::cout << "vector = ";
      const auto values = result["vector"].as<std::vector<double>>();
      for (const auto& v : values) {
        std::cout << v << ", ";
      }
      std::cout << std::endl;
    }

    std::cout << "Arguments remain = " << argc << std::endl;

    auto arguments = result.arguments();
    std::cout << "Saw " << arguments.size() << " arguments" << std::endl;

    std::cout << "Unmatched options: ";
    for (const auto& option : result.unmatched()) {
      std::cout << "'" << option << "' ";
    }
    std::cout << std::endl;
  } catch (const cxxopts::exceptions::exception& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    goto EXIT_WITH_NULLPTR;
  }

  return options;

EXIT_WITH_NULLPTR:
  options.reset();
  return options;
}

#endif  // ARG_H_
