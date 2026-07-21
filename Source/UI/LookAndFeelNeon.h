/*
  NeonSynth - Custom Look and Feel
  Neon synth UI styling for JUCE 8.0.14
*/

#pragma once

#include <JuceHeader.h>

namespace NeonSynth {

class LookAndFeelNeon : public juce::LookAndFeel_V4
{
public:
    LookAndFeelNeon() = default;

    // Override rotary slider drawing
    void drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h,
                          float sliderPos, const float rotaryStartAngle,
                          const float rotaryEndAngle, juce::Slider&) override;

    // Override text button drawing
    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                        bool isMouseOverButton, bool isMouseDraggingOutsideButton) override;

    // Neon color palette
    static juce::Colour neonCyan()   { return juce::Colour(0xff00ffff); }
    static juce::Colour neonPink()   { return juce::Colour(0xffff00aa); }
    static juce::Colour neonBlue()   { return juce::Colour(0xff4060ff); }
    static juce::Colour bgDark()     { return juce::Colour(0xff08080a); }
    static juce::Colour textWhite()  { return juce::Colour(0xffe0e0e0); }
    static juce::Colour gridLine()   { return juce::Colour(0x154060ff); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeelNeon)
};

} // namespace NeonSynth
