#ifndef NT_TYPES_H
#define NT_TYPES_H

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <stdexcept>
#include <cstdint>

#define NUM_PAT 4096
#define NUM_INS 256
#define NUM_OPF 256
#define NUM_REG 32

struct Tracker;

struct Seq;
struct Sng;
struct Pat;
struct Phr;
struct Ins;
struct OpF;
struct Op;
struct Arg;
struct Voc;

struct Ctx;

using Nam = std::string;
using Dsc = std::string;
using Smp = float;
using Tik = uint32_t;

// trigger
struct Trg {
  uint8_t val_; // bits 0-3: trigger number, bit 7: set
};

// controller
struct Ctl {
  uint8_t reg_; // bits 0-3: register index, bit 7: set
  uint8_t val_; // bits 0-7: register value
};

// advance direction
enum class AdvDir { right, down };

using VByt = std::vector<uint8_t>;
using VFlt = std::vector<float>;

// register file
struct Rfl {
  uint8_t byt_[NUM_REG];
  float flt_[NUM_REG];
  std::vector<VByt> vbyt_;
  std::vector<VFlt> vflt_;
};

enum class ArgTyp { byt = 1, flt = 2, vbyt = 4, vflt = 8 };
enum class ArgSrc { nil = 0, imm, voc, ins, phr, pat, sng, seq };

// opcode argument
struct Arg {
  ArgTyp typ_;
  ArgSrc src_;
  uint8_t val_; // literal value (if src_ == imm) or register index
};

struct ArgDsc {
  Nam nam_;
  Dsc dsc_;
  ArgTyp typ_; // bitwise OR of allowed types
};

using VArg = std::vector<Arg>;

struct BadArg : public std::runtime_error {
  Ins *ins_;
  Op *op_;
  int narg_;
  BadArg(Ins *ins, Op *op, int narg, const char *msg) 
    : std::runtime_error(msg), ins_(ins), op_(op), narg_(narg) {}
};

// opcode factory
struct OpF {
  Nam nam_;
  Dsc dsc_;
  std::vector<ArgDsc> argDsc_;
  OpF(Nam nam, Dsc dsc) : nam_(nam), dsc_(dsc) {}
  virtual Op *create() = 0;
};

// opcode
struct Op {
  virtual void trigger(Ctx *ctx, Trg trg) {}
  virtual void process(Ctx *ctx, int nf) {};
};

template <class P>
struct Instance {
  P *pro_; // prototype
  Rfl rfl_; // register file
  bool act_; // active?
  void ctl(Ctl ctl) {
    rfl_.byt_[ctl.reg_] = ctl.val_;
  }
  void play() {
    for (int i = 0; i < NUM_REG; i++) {
      rfl_.byt_[i] = pro_->rfl_.byt_[i];
      rfl_.flt_[i] = pro_->rfl_.flt_[i];
    }
    act_ = true;
  }
  void stop() {
    act_ = false;
  }
};

template <class P>
struct Tickable : public Instance<P> {
  Tik tik_;
  Tik start_;
  void play() {
    Instance<P>::play();
    tik_ = 0;
    start_ = 0;
  }
};

// prototype of instances
template <class I> struct Proto {
  Rfl rfl_;
  std::vector<int> lenVByt_;
  std::vector<int> lenVFlt_;
  uint8_t poly_ = 1;
  std::vector<std::unique_ptr<I>> obj_;
  void play() {
    obj_.resize(poly_);
    for (int i = 0; i < poly_; i++) {
      if (!obj_[i]) {
        obj_[i].reset(new I());
      }
      auto rfl = obj_[i]->rfl_;
      rfl_.vbyt_.resize(lenVByt_.size());
      for (unsigned int j = 0; j < lenVByt_.size(); j++) {
        rfl_.vbyt_[j].resize(lenVByt_[j]);
      }
      rfl_.vflt_.resize(lenVFlt_.size());
      for (unsigned int j = 0; j < lenVFlt_.size(); j++) {
        rfl_.vflt_[j].resize(lenVFlt_[j]);
      }
    }
  }
  I *alloc() {
    I* oldest = nullptr;
    for (auto &obj : obj_) {
      if (!obj->act_) {
        return obj.get();
      }
      if (!oldest || obj->start_ < oldest->start_) {
        oldest = obj.get();
      }
    }
    return oldest;
  }
};

// instrument
struct Ins : public Proto<Voc> {
  std::vector<OpF*> opf_;
  std::vector<VArg> vargs_;
  bool isBus_;
};

// voice
struct Voc : public Instance<Ins> {
  std::vector<Op*> ops_;
  Tik start_;
  void trigger(Ctx *ctx, Trg trg);
  void process(Ctx *ctx, int nf);
};

// pattern packet
struct PatPkt {
  Ins *ins_;
  std::vector<Trg> trg_;
  std::vector<Ctl> ctl_;
};

// pattern channel
struct Chn {
  std::vector<PatPkt> pkt_;
  Voc *voc_; // current channel voice
};

// pattern
struct Pat : public Proto<Phr> {
  int lpb_; // lines per beat
  std::vector<Chn> chn_;
};

// phrase
struct Phr : public Tickable<Pat> {
  Pat *pat_;
  void tick(Ctx *ctx);
};

// track packet
struct TrkPkt {
  Pat *pat_;
  std::vector<Trg> trg_;
  std::vector<Ctl> ctl_;
};

// track
struct Trk {
  std::vector<TrkPkt> pkt_;
  Phr *phr_; // current track phrase
};

// sequencer
struct Seq : public Proto<Sng> {
  int bpr_;
  std::vector<Trk> trk_;
};

// song
struct Sng : public Tickable<Seq> {
  Seq *seq_;
  void tick(Ctx *ctx);
};

#endif
