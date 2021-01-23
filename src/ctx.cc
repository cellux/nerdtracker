#include <stdexcept>

#include "ctx.h"

Rfl *Ctx::rfl(const Arg &arg) {
  switch (arg.src_) {
  case ArgSrc::voc:
    return &voc_->rfl_;
  case ArgSrc::ins:
    return &ins_->rfl_;
  case ArgSrc::phr:
    return &phr_->rfl_;
  case ArgSrc::pat:
    return &pat_->rfl_;
  case ArgSrc::sng:
    return &sng_->rfl_;
  case ArgSrc::seq:
    return &seq_->rfl_;
  default:
    throw std::runtime_error("invalid arg src");
  }
}

uint8_t Ctx::byt(int narg) {
  const Arg &arg = varg_->operator[](narg);
  if (arg.src_ == ArgSrc::imm) {
    return arg.val_;
  }
  else {
    int idx = arg.val_;
    return rfl(arg)->byt_[idx];
  }
}

float Ctx::flt(int narg) {
  const Arg &arg = varg_->operator[](narg);
  int idx = arg.val_;
  return rfl(arg)->flt_[idx];
}

VByt &Ctx::vbyt(int narg) {
  const Arg &arg = varg_->operator[](narg);
  int idx = arg.val_;
  return rfl(arg)->vbyt_[idx];
}

VFlt &Ctx::vflt(int narg) {
  const Arg &arg = varg_->operator[](narg);
  int idx = arg.val_;
  return rfl(arg)->vflt_[idx];
}

void Ctx::vouts(int narg, VFlt **vflt, int nchannels) {
  const Arg &arg = varg_->operator[](narg);
  if (arg.src_ == ArgSrc::nil) {
    auto r = &voc0_->rfl_;
    for (int i = 0; i < nchannels; i++) {
      vflt[i] = &r->vflt_[i];
    }
  } else {
    int idx = arg.val_;
    auto r = rfl(arg);
    for (int i = 0; i < nchannels; i++) {
      vflt[i] = &r->vflt_[idx++];
    }
  }
}
