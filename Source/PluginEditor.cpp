/*
  NeonSynth - Plugin Editor
  Main UI with neon styling
  JUCE 8 compatible
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "UI/LookAndFeelNeon.h"
#include "UI/Oscilloscope.h"
#include "UI/FilterEditor.h"
#include "UI/EnvelopeEditor.h"
#include "UI/PresetBrowser.h"

namespace NeonSynth {

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(p),
      processor_(p)
{
    // Set custom neon look and feel
    setLookAndFeel(&lookAndFeel_);
}

PluginEditor::~PluginEditor()
{
    setLookAndFeel(nullptr);
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff08080a));
}

void PluginEditor::resized()
{
    // Layout will be handled by component positioning
}

} // namespace NeonSynth
