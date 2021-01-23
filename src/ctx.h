#ifndef NT_CTX_H
#define NT_CTX_H

#include <cstdint>

#include "types.h"

struct Ctx {
  Tracker *t_;
  Seq *seq_;
  Sng *sng_;
  Pat *pat_;
  Phr *phr_;
  Ins *ins_;
  Op *op_;
  VArg *varg_;
  Voc *voc_;
  Voc *voc0_;
  Rfl *rfl(const Arg &arg);
  uint8_t byt(int narg);
  float flt(int narg);
  VByt &vbyt(int narg);
  VFlt &vflt(int narg);
  void vouts(int narg, VFlt **vflt, int nchannels);
};

#endif
