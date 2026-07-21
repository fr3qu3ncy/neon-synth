/*
  NeonSynth - Oscilloscope Visualization
  Real-time waveform display
  JUCE 8 compatible
*/

#pragma once

#include <JuceHeader.h>

namespace NeonSynth {

class Oscilloscope : public juce::Component
{
public:
    Oscilloscope();
    ~Oscilloscope() override;

    void update(const float* data, int numSamples);
    void paint(juce::Graphics& g) override;

private:
    std::vector<float> waveform_;
    juce::CriticalSection waveLock_;
};

} // namespace NeonSynth
