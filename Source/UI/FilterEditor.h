/*
  NeonSynth - Filter Editor
  Frequency response curve visualization with neon styling
*/

#pragma once

#include <JuceHeader.h>

namespace NeonSynth {

class FilterEditor : public juce::Component
{
public:
    FilterEditor();
    ~FilterEditor() override;

    void setCutoff(double hz);
    void setResonance(double res);
    void setType(int type); // 0=LP, 1=HP, 2=BP

    void paint(juce::Graphics&) override;

private:
    double cutoff_ = 1000.0;
    double resonance_ = 0.0;
    int filterType_ = 0;

    double getResponse(double freq) const;
};

} // namespace NeonSynth
