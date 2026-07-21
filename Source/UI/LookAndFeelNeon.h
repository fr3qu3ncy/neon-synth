/*
  NeonSynth - Neon LookAndFeel
  Custom theme: deep charcoal bg, cyan/magenta neon accents
*/

#pragma once

#include <JuceHeader.h>

namespace NeonSynth {

class LookAndFeelNeon : public juce::LookAndFeel_V4
{
public:
    LookAndFeelNeon();

    // Colours
    juce::Colour bgDark = juce::Colour(0xff0d0d0f);
    juce::Colour neonCyan = juce::Colour(0xff00f0ff);
    juce::Colour neonMagenta = juce::Colour(0xffff00aa);
    juce::Colour neonBlue = juce::Colour(0xff4060ff);
    juce::Colour textWhite = juce::Colour(0xfff0f0f0);
    juce::Colour dimText = juce::Colour(0xff666670);

    void drawRotarySlider(juce::Graphics&, int x, int y, int w, int h,
                          float sliderPos, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider&) override;
    void drawButtonText(juce::Graphics&, juce::Button&,
                        bool isMouseOverButton, bool isMouseDown) override;
    void drawComboBox(juce::Graphics&, int w, int h, bool,
                      int, int, int, int, juce::ComboBox&) override;
};

} // namespace NeonSynth
