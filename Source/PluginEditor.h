/*
  NeonSynth - PluginEditor
  Main UI with neon theme
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

namespace NeonSynth {

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    PluginProcessor& processor_;

    // UI components will be added here
    // - Oscillator panels
    // - Envelope editors
    // - Filter section
    // - Oscilloscope
    // - Preset browser

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};

} // namespace NeonSynth
