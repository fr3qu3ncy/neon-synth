/*
  NeonSynth - Preset Browser
  Load/save presets using ValueTree serialization
  JUCE 8 compatible
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

namespace NeonSynth {

class PresetBrowser : public juce::Component
{
public:
    PresetBrowser(PluginProcessor& processor);
    ~PresetBrowser() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void loadPreset(const juce::File& file);
    void savePreset(const juce::File& file);

    PluginProcessor& processor;
    juce::TextButton loadButton;
    juce::TextButton saveButton;
    juce::TextButton browseButton;
    juce::Label presetNameLabel;

    juce::File getPresetDirectory() const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetBrowser)
};

} // namespace NeonSynth
