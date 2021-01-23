#include "audio.h"

cli::Option<int> audioSampleRateOption_ {"-r,--audio-sample-rate"};
cli::Option<int> audioDeviceIdOption_ {"-d,--audio-device-id"};
cli::Option<int> audioBufferFramesOption_ {"-b,--audio-buffer-frames"};

template <> RtAudioFormat getRtAudioFormat<float>() {
  return RTAUDIO_FLOAT32;
}

template <> RtAudioFormat getRtAudioFormat<double>() {
  return RTAUDIO_FLOAT64;
}

void rtAudioErrorCallback(RtAudioError::Type type,
                          const std::string &errorText) {
  std::cerr << "[RTAUDIO] " << errorText << "\n";
}
