#include "types.h"
#include "ctx.h"

namespace opf {

struct Noise : public OpF {
  struct impl : public Op {
    void process(Ctx *ctx, int nf);
  };
  Noise() : OpF("NOI", "Noise generator") {
    argDsc_.push_back(ArgDsc{
        "OUT", "Destination buffer", ArgTyp::vflt
      });
  }
  Op *create() { return new impl(); }
};

} // namespace opf
