/*
  NeonSynth - Oscilloscope Component
  OpenGL-based real-time waveform visualization
*/

#pragma once

#include <JuceHeader.h>

namespace NeonSynth {

class Oscilloscope : public juce::Component
{
public:
    Oscilloscope();
    ~OscillatorScope() override;

    void paint(juce::Graphics&) override;
    void update(const std::vector<float>& osc1Data, const std::vector<float>& osc2Data);

private:
    std::vector<float> osc1Buffer_;
    std::vector<float> osc2Buffer_;
    juce::CriticalSection bufferLock_;
};

} // namespace NeonSynth
