/*
  NeonSynth - Polyphonic Voice Manager
  16-voice polyphony with voice stealing
*/

#pragma once

#include "Voice.h"
#include <array>

namespace NeonSynth {

class VoiceManager
{
public:
    static constexpr int maxVoices_ = 16;

    VoiceManager();
    ~VoiceManager() = default;

    void setSampleRate(double rate);

    /** Note on - allocates voice or steals one */
    void noteOn(double note, double velocity);

    /** Note off - starts release phase */
    void noteOff(double note);

    /** Process one sample, writes to stereo outputs */
    void processSample(double& outLeft, double& outRight);

    /** Get voice at index (for oscilloscope access) */
    Voice* getVoice(int index);

    /** Get number of active voices */
    int getActiveVoiceCount() const;

private:
    void stealVoice();

    std::array<Voice, maxVoices_> voices_;
    std::array<bool, maxVoices_> voiceFree_;
    std::array<double, maxVoices_> voiceNote_;
    int activeVoices_ = 0;
};

} // namespace NeonSynth
