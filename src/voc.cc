#include "types.h"
#include "ctx.h"

void Voc::trigger(Ctx *ctx, Trg trg) {
  Ins *ins = pro_;
  ctx->voc_ = this;
  ctx->ins_ = ins;
  for (unsigned int i = 0; i < ops_.size(); i++) {
    auto op = ops_[i];
    ctx->op_ = op;
    ctx->varg_ = &ins->vargs_[i];
    op->trigger(ctx, trg);
  }
}

void Voc::process(Ctx *ctx, int nf) {
  Ins *ins = pro_;
  ctx->voc_ = this;
  ctx->ins_ = ins;
  for (unsigned int i = 0; i < ops_.size(); i++) {
    auto op = ops_[i];
    ctx->op_ = op;
    ctx->varg_ = &ins->vargs_[i];
    op->process(ctx, nf);
  }
};
