#include "cli.h"

namespace cli {

ShortOptions &shortOptions() {
  static ShortOptions opts;
  return opts;
}

LongOptions &longOptions() {
  static LongOptions opts;
  return opts;
}

} // namespace cli
