/*
  NeonSynth - Preset Browser Implementation
*/

#include "UI/PresetBrowser.h"

namespace NeonSynth {

PresetBrowser::PresetBrowser()
{
    addAndMakeVisible(presetSelector_);
    addAndMakeVisible(presetNameEditor_);

    presetSelector_.setTextWhenNothingSelected("< select preset >");
    presetSelector_.setJustification(juce::Justification::centred);

    presetNameEditor_.setPlaceholder("Preset name...");
    presetNameEditor_.setFont(juce::Font(12.0f));

    refreshPresetList();
}

PresetBrowser::~PresetBrowser() = default;

void PresetBrowser::refreshPresetList()
{
    presetNames_.clear();
    auto dir = getPresetDir();

    if (dir.exists())
    {
        auto finder = dir.findChildFiles(juce::File::findFiles, false, "*.json");
        for (const auto& file : finder)
            presetNames_.add(file.getFileNameWithoutExtension());
    }

    presetSelector_.clear(0);
    for (const auto& name : presetNames_)
        presetSelector_.addItem(name, 1);
}

juce::File PresetBrowser::getPresetDir()
{
    return juce::File::specialFolder Applications
        .getChildFile("NeonSynth")
        .getChildFile("Presets");
}

bool PresetBrowser::loadPreset(const juce::File& file)
{
    if (!file.existsAsFile())
        return false;

    auto xml = juce::XMLDocument::parse(file);
    if (!xml)
        return false;

    // TODO: Apply XML state to processor parameters
    return true;
}

bool PresetBrowser::savePreset(const juce::String& name)
{
    auto dir = getPresetDir();
    dir.createDirectory();

    auto file = dir.getChildFile(name + ".json");

    // TODO: Serialize current processor state to JSON
    juce::ValueTree state("NeonSynthPreset");
    state.writeToFile(file, nullptr);

    refreshPresetList();
    return true;
}

void PresetBrowser::deletePreset()
{
    if (selectedIndex_ < 0 || selectedIndex_ >= presetNames_.size())
        return;

    auto dir = getPresetDir();
    auto file = dir.getChildFile(presetNames_[selectedIndex_] + ".json");
    file.deleteFile();

    refreshPresetList();
}

void PresetBrowser::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff08080a));

    // Border glow
    g.setColour(juce::Colour(0x3000f0ff));
    g.drawRect(getLocalBounds(), 2);

    // Title
    g.setColour(juce::Colour(0xff00f0ff));
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText("PRESETS", 10, 8, 80, 16, juce::Justification::centredLeft);
}

void PresetBrowser::resized()
{
    presetSelector_.setBounds(10, 30, getWidth() - 20, 25);
    presetNameEditor_.setBounds(10, 65, getWidth() / 2 - 15, 22);
}

} // namespace NeonSynth
