/*
  NeonSynth - Preset Browser
  Save/load/delete presets with neon UI
*/

#pragma once

#include <JuceHeader.h>

namespace NeonSynth {

class PresetBrowser : public juce::Component
{
public:
    PresetBrowser();
    ~PresetBrowser() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    /** Load preset from file */
    bool loadPreset(const juce::File& file);

    /** Save current state as preset */
    bool savePreset(const juce::String& name);

    /** Delete selected preset */
    void deletePreset();

    /** Get preset directory */
    juce::File getPresetDir();

private:
    juce::ComboBox presetSelector_;
    juce::TextEditor presetNameEditor_;
    juce::StringArray presetNames_;
    int selectedIndex_ = -1;
};

} // namespace NeonSynth
