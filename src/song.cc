#include "types.h"
#include "tracker.h"
#include "ctx.h"

void Sng::tick(Ctx *ctx) {
  Tracker *t = ctx->t_;
  ctx->sng_ = this;
  int tpr = seq_->bpr_ * t->tpb_;
  if (tik_ % tpr == 0) {
    int row = tik_ / tpr;
    for (auto trk : seq_->trk_) {
      auto pkt = trk.pkt_[row];
      if (pkt.pat_) {
        auto phr = pkt.pat_->alloc();
        if (!phr->act_) {
          t->phr_.push_back(phr);
        }
        trk.phr_ = phr;
        phr->play();
      }
      auto phr = trk.phr_;
      //for (auto trg : pkt.trg_) {}
      for (auto ctl : pkt.ctl_) {
        phr->ctl(ctl);
      }
    }
  }
  tik_++;
}
