#ifndef NT_TRACKER_H
#define NT_TRACKER_H

#include <memory>

#include "types.h"
#include "ctx.h"
#include "audio.h"

#define MIN_BPM 10
#define MIN_TPB 1

struct Tracker : public AudioSource<Smp> {
  unsigned int sr_;
  unsigned int nchannels_;
  unsigned int nfMax_;
  int bpm_ = 120;
  int tpb_ = 24;
  std::unique_ptr<Seq> seq_;
  std::vector<Sng*> sng_;
  std::vector<std::unique_ptr<Pat>> pat_ { NUM_PAT };
  std::vector<Phr*> phr_;
  std::vector<std::unique_ptr<Ins>> ins_ { NUM_INS };
  std::vector<std::unique_ptr<OpF>> opf_ { NUM_OPF };
  std::vector<Voc*> voc_;
  std::vector<Voc *> bus_;
  bool act_ = false;
  Tik tik_; // current tick
  float tph_; // tick phase [0,1)
  float fpt_; // sample frames per tick
  Ctx ctx_;
  Tracker(unsigned int sr, unsigned int nchannels, unsigned int nfMax);
  Ins* getOrCreateIns(int index);
  void tick();
  void play();
  void playSeq(int row);
  void playPat(int npat, int line);
  void playPkt(PatPkt *pkt);
  void stop();
  void process(Smp *out, int nf);
  void mixDown(Smp *out, unsigned int nf);
};

#endif
