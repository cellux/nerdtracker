#ifndef NT_AUDIO_H
#define NT_AUDIO_H

#include "cli.h"

#include <rtaudio/RtAudio.h>

extern cli::Option<int> audioSampleRateOption_;
extern cli::Option<int> audioDeviceIdOption_;
extern cli::Option<int> audioBufferFramesOption_;

template <typename Smp> RtAudioFormat getRtAudioFormat();

template <typename Smp>
class AudioSource {
public:
  virtual void mixDown(Smp *outputBuffer, unsigned int nFrames) = 0;
};

template <typename Smp> class AudioManager;

template <typename Smp>
int rtAudioCallback(void *outputBuffer,
                    void *inputBuffer,
                    unsigned int nFrames,
                    double streamTime,
                    RtAudioStreamStatus status,
                    void *userData) {
  auto mgr = static_cast<AudioManager<Smp>*>(userData);
  auto buf = static_cast<Smp*>(outputBuffer);
  auto nChannels = mgr->nChannels();
  for (unsigned int i = 0; i < nFrames * nChannels; i++) {
    buf[i] = 0;
  }
  mgr->mixDown(buf, nFrames);
  return mgr->isRunning() ? 0 : 1;
}

void rtAudioErrorCallback(RtAudioError::Type type,
                          const std::string &errorText);

template <typename Smp> class AudioManager : public AudioSource<Smp> {

  RtAudio rtAudio_;
  unsigned int sampleRate_;
  unsigned int deviceId_;
  unsigned int nChannels_ {2};
  unsigned int bufferFrames_;
  std::vector<AudioSource<Smp> *> audioSources_;

public:
  AudioManager() {
    std::cout << "Using audio API: " << RtAudio::getApiDisplayName(rtAudio_.getCurrentApi()) << "\n";
    deviceId_ = audioDeviceIdOption_.value(rtAudio_.getDefaultOutputDevice());
  if (deviceId_ == (unsigned int) -1) {
    std::cout << "Available audio output devices:\n";
    for (unsigned int i=0; i<rtAudio_.getDeviceCount(); i++) {
      auto info = rtAudio_.getDeviceInfo(i);
      std::cout << i << ": " << info.name << "\n";
    }
    std::exit(0);
  }
  if (deviceId_ >= rtAudio_.getDeviceCount()) {
    throw std::runtime_error("invalid audio device id");
  }
  sampleRate_ = audioSampleRateOption_.value(48000);
  RtAudioFormat audioFormat = getRtAudioFormat<Smp>();
  bufferFrames_ = audioBufferFramesOption_.value(0);
  struct RtAudio::StreamParameters sp;
  sp.deviceId = deviceId_;
  sp.nChannels = nChannels_;
  struct RtAudio::StreamOptions so;
  so.streamName = "NerdTracker AUDIO Out";
  rtAudio_.openStream(&sp, 0, audioFormat,
                      sampleRate_, &bufferFrames_,
                      rtAudioCallback<Smp>, this, 
                      &so, rtAudioErrorCallback);
  }
  ~AudioManager() {
    stop();
    if (rtAudio_.isStreamOpen()) {
      rtAudio_.closeStream();
    }
  };
  void start() {
    if (!isRunning()) {
      rtAudio_.startStream();
    }
  }
  void stop() {
    if (isRunning()) {
      rtAudio_.stopStream();
    }
  }
  unsigned int nChannels() const { return nChannels_; }
  unsigned int sampleRate() const { return sampleRate_; }
  unsigned int bufferFrames() const { return bufferFrames_; }
  bool isRunning() const { return rtAudio_.isStreamRunning(); }
  void addSource(AudioSource<Smp> *src) { audioSources_.push_back(src); }
  void mixDown(Smp *outputBuffer, unsigned int nFrames) {
    for (auto src : audioSources_) {
      src->mixDown(outputBuffer, nFrames);
    }
  }
};

#endif
