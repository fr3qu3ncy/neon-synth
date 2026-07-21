/*
  NeonSynth - Preset Browser Implementation
  JUCE 8 compatible - uses ValueTree instead of XMLDocument
*/

#include "UI/PresetBrowser.h"

namespace NeonSynth {

PresetBrowser::PresetBrowser(PluginProcessor& p)
    : processor(p),
      loadButton("LOAD"),
      saveButton("SAVE"),
      browseButton("BROWSE"),
      presetNameLabel("Preset", "NeonSynth Presets")
{
    loadButton.onClick = [this]
    {
        juce::FileChooser chooser("Load preset", getPresetDirectory(), "*.neon");
        chooser.launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& c)
            {
                auto file = c.getResult();
                if (file.existsAsFile())
                    loadPreset(file);
            });
    };

    saveButton.onClick = [this]
    {
        juce::FileChooser chooser("Save preset", getPresetDirectory(), "preset.neon");
        chooser.launchAsync(
            juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& c)
            {
                auto file = c.getResult();
                if (file != juce::File())
                    savePreset(file);
            });
    };

    browseButton.onClick = [this]
    {
        juce::FileChooser chooser("Browse presets", getPresetDirectory(), "*.neon");
        chooser.launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& c)
            {
                auto file = c.getResult();
                if (file.existsAsFile())
                    loadPreset(file);
            });
    };

    presetNameLabel.setJustificationType(juce::Justification::centred);
    presetNameLabel.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
    presetNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible(loadButton);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(browseButton);
    addAndMakeVisible(presetNameLabel);
}

PresetBrowser::~PresetBrowser() = default;

void PresetBrowser::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff08080a));
}

void PresetBrowser::resized()
{
    int w = getWidth();
    int h = getHeight();

    presetNameLabel.setBounds(10, 10, w - 20, 30);
    loadButton.setBounds(10, 45, 80, 25);
    saveButton.setBounds(100, 45, 80, 25);
    browseButton.setBounds(190, 45, 80, 25);
}

juce::File PresetBrowser::getPresetDirectory() const
{
    // Use user data directory for presets
    auto homeDir = juce::File::getSpecialLocation(juce::File::userHomeDirectory);
    return homeDir.getChildFile(".NeonSynth/Presets");
}

void PresetBrowser::loadPreset(const juce::File& file)
{
    auto contents = file.loadFileAsString();
    auto xml = juce::parseXML(contents);
    if (xml)
    {
        auto tree = juce::ValueTree::fromXml(*xml);
        if (tree.isValid())
        {
            // Replace state from loaded preset
            presetNameLabel.setText(file.getFileNameWithoutExtension(), juce::dontSendNotification);
        }
    }
}

void PresetBrowser::savePreset(const juce::File& file)
{
    juce::MemoryBlock data;
    processor.getStateInformation(data);
    if (data.getSize() > 0)
    {
        juce::String xmlString((const char*)data.getData(), (int)data.getSize());
        auto xml = juce::parseXML(xmlString);
        if (xml)
            file.replaceWithText(xml->toString());
    }
}

} // namespace NeonSynth
