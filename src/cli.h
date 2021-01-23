#ifndef NT_CLI_H
#define NT_CLI_H

#include <string>
#include <map>
#include <optional>
#include <iostream>
#include <sstream>

namespace cli {

class OptionBase {
public:
  virtual void parseValue(const char *s) = 0;
};

using ShortOptions = std::map<char, OptionBase*>;
ShortOptions &shortOptions();

using LongOptions = std::map<std::string, OptionBase*>;
LongOptions &longOptions();

struct OptionParser {
  OptionParser(int argc, char **argv) {
    int i = 1;
    while (i < argc) {
      const char *arg = argv[i++];
      if (arg[0] == '-') {
        if (arg[1] == '-') {
          std::string longName{&arg[2]};
          auto &opts = longOptions();
          auto item = opts.find(longName);
          if (item != opts.end()) {
            if (i >= argc) {
              throw std::runtime_error("no value for long option");
            }
            OptionBase *option = item->second;
            option->parseValue(argv[i++]);
          } else {
            std::cerr << "unknown long option: " << arg << "\n";
          }
        } else {
          char shortName = arg[1];
          auto &opts = shortOptions();
          auto item = opts.find(shortName);
          if (item != opts.end()) {
            if (i >= argc) {
              throw std::runtime_error("no value for short option");
            }
            OptionBase *option = item->second;
            option->parseValue(argv[i++]);
          }
          else {
            std::cerr << "unknown short option: " << arg << "\n";
          }
        }
      }
    }
  }
};

template <typename T> class Option : public OptionBase {
  std::optional<T> value_;

public:
  Option(const std::string &names) {
    std::istringstream is(names);
    std::string opt;
    while (getline(is, opt, ',')) {
      if (opt.size() < 2 || opt[0] != '-') {
        throw std::runtime_error("invalid option");
      }
      if (opt[1] == '-') {
        if (opt.size() < 3) {
          throw std::runtime_error("invalid long option");
        }
        auto &opts = longOptions();
        opts[opt.substr(2)] = this;
      } else {
        if (opt.size() != 2) {
          throw std::runtime_error("invalid short option");
        }
        auto &opts = shortOptions();
        opts[opt[1]] = this;
      }
    }
  }

  void parseValue(const char *s) {
    std::istringstream is(s);
    T v;
    is >> v;
    value_ = v;
  }

  const T value(const T &defaultValue) const {
    return value_.value_or(defaultValue);
  }
};

} // namespace cli

#endif
