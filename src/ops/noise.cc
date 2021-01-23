#include <cmath>

#include "ops/noise.h"

#define ARG_OUT 0

namespace opf {

void Noise::impl::process(Ctx *ctx, int nf) {
  VFlt *outs[1];
  ctx->vouts(ARG_OUT, outs, 1);
  float *out = outs[0]->data();
  for (int i=0; i<nf; i++) {
    out[i] += 1.0 - 2*(float(rand()) / RAND_MAX);
  }
}

} // namespace opf
