#include <cmath>

#include "tracker.h"
#include "ctx.h"
#include "util.h"

#include "ops/noise.h"

template <typename T> void flush(std::vector<T*> &v) {
  for (auto el : v) {
    el->stop();
  }
  v.clear();
}

Tracker::Tracker(unsigned int sr, unsigned int nchannels, unsigned int nfMax) : sr_(sr), nchannels_(nchannels), nfMax_(nfMax) {
  LOG("created tracker: sr=%d nchannels=%d nfMax=%d", sr, nchannels, nfMax);
  seq_ = std::make_unique<Seq>();
  auto ins0 = new Ins();
  ins0->isBus_ = true;
  for (unsigned int i = 0; i < nchannels; i++) {
    ins0->lenVFlt_.push_back(nfMax_);
  }
  ins_[0].reset(ins0);
  opf_[0x01].reset(new opf::Noise());

  ctx_.t_ = this;
  ctx_.seq_ = seq_.get();
}

Ins *Tracker::getOrCreateIns(int index) {
  if (!ins_.at(index)) {
    ins_[index].reset(new Ins());
  }
  return ins_[index].get();
}

void Tracker::tick() {
  for (auto sng : sng_) {
    sng->tick(&ctx_);
  }
  for (auto phr : phr_) {
    phr->tick(&ctx_);
  }
  fpt_ = sr_ / ((bpm_ * tpb_) / 60.0);
  tik_++;
}

void Tracker::play() {
  if (act_) return;
  voc_.clear();
  bus_.clear();
  for (auto &ins : ins_) {
    ins->play();
    if (ins->isBus_) {
      auto voc = ins->alloc();
      bus_.push_back(voc);
    }
  }
  ctx_.voc0_ = ins_[0]->obj_[0].get();
  tik_ = 0;
  tph_ = 0;
  act_ = true;
}

void Tracker::playSeq(int row) {
  seq_->play();
  auto sng = seq_->alloc();
  if (!sng->act_) {
    sng_.push_back(sng);
  }
  sng->tik_ = tpb_ * seq_->bpr_ * row;
  sng->play();
  play();
}

void Tracker::playPat(int npat, int line) {
  auto pat = pat_.at(npat).get();
  if (!pat) return;
  pat->play();
  auto phr = pat->alloc();
  if (!phr->act_) {
    phr_.push_back(phr);
  }
  phr->tik_ = (tpb_ * line) / pat->lpb_;
  phr->play();
  play();
}

void Tracker::playPkt(PatPkt *pkt) {
  auto ins = pkt->ins_;
  if (!ins)
    return;
  ins->play();
  auto voc = ins->alloc();
  if (!voc->act_) {
    voc_.push_back(voc);
  }
  voc->start_ = tik_;
  for (auto ctl : pkt->ctl_) {
    voc->ctl(ctl);
  }
  voc->trigger(&ctx_, Trg{1});
  for (auto trg : pkt->trg_) {
    if (trg.val_ != 1) {
      voc->trigger(&ctx_, trg);
    }
  }
  voc->play();
  play();
}

void Tracker::stop() {
  act_ = false;
  flush(sng_);
  flush(phr_);
  flush(voc_);
  flush(bus_);
}

void Tracker::process(Smp *out, int nf) {
  for (auto voc : voc_) {
    voc->process(&ctx_, nf);
  }
  for (auto bus : bus_) {
    bus->process(&ctx_, nf);
  }
  auto &voc0 = ins_[0]->obj_[0];
  auto &channels = voc0->rfl_.vflt_;
  if (channels.size() < nchannels_) {
    DIE("audio output needs %d channels but INS 00 provides only %d", nchannels_, channels.size());
  }
  for (unsigned int i = 0; i < nchannels_; i++) {
    int size = channels[i].size();
    if (size < nf) {
      DIE("audio output needs %d sample frames but INS 00 channel %d provides only %d", nf, i, size);
    }
  }
  int index = 0;
  for (int i = 0; i < nf; i++) {
    for (unsigned int j = 0; j < nchannels_; j++) {
      out[index++] += channels[j][i];
    }
  }
}

void Tracker::mixDown(Smp *outputBuffer, unsigned int nFrames) {
  if (!act_) return;
  int rem = nFrames; // number of remaining frames
  float nf; // number of sample frames left from the current tick
  while (rem > 0) {
    if (tph_ == 0) {
      tick();
    }
    nf = fpt_ * (1.0 - tph_);
    if (nf > rem) {
      process(outputBuffer, rem);
      tph_ += (rem / fpt_);
      rem = 0;
    } else {
      int inf = round(nf);
      if (inf > 0) {
        process(outputBuffer, inf);
        rem -= inf;
      }
      tph_ = 0;
    }
  }
}
