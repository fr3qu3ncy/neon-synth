/*
  NeonSynth - LookAndFeelNeon Implementation
  JUCE 8.0.14 compatible
*/

#include "UI/LookAndFeelNeon.h"

namespace NeonSynth {

//==============================================================================
void LookAndFeelNeon::drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h,
                                       float sliderPos, const float /*rotaryStartAngle*/,
                                       const float /*rotaryEndAngle*/, juce::Slider& /*slider*/)
{
    const float radius = std::min(w, h) * 0.5f;
    const float centreX = x + w * 0.5f;
    const float centreY = y + h * 0.5f;

    // Background ring
    g.setColour(gridLine());
    g.drawEllipse(x + 2, y + 2, w - 4, h - 4, 2.0f);

    // Value arc
    g.setColour(neonCyan());
    const float startAngle = juce::MathConstants<float>::pi * 1.2f;
    const float endAngle = startAngle + sliderPos * (juce::MathConstants<float>::pi * 1.6f);
    juce::Path arc;
    arc.addCentredArc(centreX, centreY, radius - 4, radius - 4, 0.0f,
                      startAngle, endAngle, true);
    g.fillPath(arc);

    // Knob indicator
    const float knobAngle = startAngle + sliderPos * (juce::MathConstants<float>::pi * 1.6f);
    const float knobX = centreX + std::cos(knobAngle) * (radius - 8);
    const float knobY = centreY + std::sin(knobAngle) * (radius - 8);
    g.setColour(neonPink());
    g.fillEllipse(knobX - 3, knobY - 3, 6, 6);
}

//==============================================================================
void LookAndFeelNeon::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                     bool isMouseOverButton, bool /*isMouseDraggingOutsideButton*/)
{
    const float alpha = isMouseOverButton ? 1.0f : 0.8f;
    g.setColour(neonCyan().withMultipliedAlpha(alpha));
    g.drawRoundedRectangle(button.getBounds().toFloat().reduced(1), 4, 1.0f);

    g.setColour(textWhite().withMultipliedAlpha(alpha));
    g.drawFittedText(button.getButtonText(), button.getLocalBounds(),
                    juce::Justification::centred, 1);
}

} // namespace NeonSynth
