/*
  NeonSynth - PluginEditor Implementation
  Neon-themed UI layout
*/

#include "PluginEditor.h"

namespace NeonSynth {

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processor_(p)
{
    setSize(800, 500);
    setWantsKeyboardFocus(true);

    // Background color
    getLookAndFeel().setColour(juce::Component::backgroundColourId,
        juce::Colour(0xff0d0d0f));
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    // Deep charcoal background
    g.fillAll(juce::Colour(0xff0d0d0f));

    // Subtle grid pattern for depth
    g.setColour(juce::Colour(0x10ffffff));
    for (int x = 0; x < getWidth(); x += 20)
        g.drawLine(x, 0, x, getHeight(), 1.0f);
    for (int y = 0; y < getHeight(); y += 20)
        g.drawLine(0, y, getWidth(), y, 1.0f);

    // Title
    g.setColour(juce::Colour(0xff00f0ff)); // Cyan neon
    g.setFont(juce::Font(18.0f, juce::Font::bold));
    g.drawFittedText("NEON SYNTH", getLocalBounds().withTrimmedRight(200),
        juce::Justification::topLeft, 1);
}

void PluginEditor::resized()
{
    // Layout will be implemented with proper component positioning
    // Phase 2 of the build plan
}

} // namespace NeonSynth

// ---- Plugin factory ----
namespace NeonSynth { extern PluginProcessor* p; }

BEGIN_JUCE_PLUGIN_CLIENT(NeonLabs)
    BEGIN_JUCE_PLUGIN_CODE(NeonSynth)
        REGISTER_AUDIO_PROCESSOR(NeonSynth::PluginProcessor, 0x00100001, false)
END_JUCE_PLUGIN_CODE
END_JUCE_PLUGIN_CLIENT
