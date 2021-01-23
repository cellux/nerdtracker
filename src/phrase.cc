#include "types.h"
#include "tracker.h"
#include "ctx.h"

void Phr::tick(Ctx *ctx) {
  Tracker *t = ctx->t_;
  ctx->phr_ = this;
  ctx->pat_ = pat_;
  int tpl = t->tpb_ / pat_->lpb_;
  if (tik_ % tpl == 0) {
    int line = tik_ / tpl;
    for (auto chn : pat_->chn_) {
      auto pkt = chn.pkt_[line];
      if (pkt.ins_) {
        chn.voc_ = pkt.ins_->alloc();
        if (!chn.voc_->act_) {
          t->voc_.push_back(chn.voc_);
        }
        chn.voc_->start_ = t->tik_;
        chn.voc_->play();
      }
      auto voc = chn.voc_;
      for (auto ctl : pkt.ctl_) {
        voc->ctl(ctl);
      }
      voc->trigger(ctx, Trg{1});
      for (auto trg : pkt.trg_) {
        if (trg.val_ != 1) {
          voc->trigger(ctx, trg);
        }
      }
    }
  }
  tik_++;
}
