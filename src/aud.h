#ifndef NT_AUD_H
#define NT_AUD_H

#include "types.h"
#include "audio.h"

struct Aud {
  AudioManager<Smp> mgr_;
  unsigned int sr() const { return mgr_.sampleRate(); }
  unsigned int nchannels() const { return mgr_.nChannels(); }
  unsigned int nf() const { return mgr_.bufferFrames(); }
  void addSource(AudioSource<Smp> *src) { mgr_.addSource(src); }
  void start() { mgr_.start(); }
  void stop() { mgr_.stop(); }
  bool isRunning() { return mgr_.isRunning(); }
};

#endif
