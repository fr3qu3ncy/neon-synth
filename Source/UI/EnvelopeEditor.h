/*
  NeonSynth - Envelope Editor
  Draggable ADSR visualization with neon styling
*/

#pragma once

#include <JuceHeader.h>

namespace NeonSynth {

class EnvelopeEditor : public juce::Component
{
public:
    EnvelopeEditor(const juce::String& title);
    ~EnvelopeEditor() override;

    void setAttack(double s);
    void setDecay(double s);
    void setSustain(double level);
    void setRelease(double s);

    void paint(juce::Graphics&) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;

private:
    juce::String title_;
    double attack_ = 0.01;
    double decay_ = 0.3;
    double sustain_ = 0.7;
    double release_ = 0.5;
};

} // namespace NeonSynth
