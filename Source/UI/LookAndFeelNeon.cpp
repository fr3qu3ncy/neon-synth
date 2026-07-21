/*
  NeonSynth - Neon LookAndFeel Implementation
*/

#include "UI/LookAndFeelNeon.h"

namespace NeonSynth {

LookAndFeelNeon::LookAndFeelNeon()
{
    // Set global colours
    setColour(juce::Slider::trackColourId, juce::Colour(0x3000f0ff));
    setColour(juce::Slider::thumbColourId, neonCyan);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x4000f0ff));
    setColour(juce::ComboBox::outlineColourId, neonCyan);
    setColour(juce::ComboBox::buttonColourId, bgDark);
    setColour(juce::ComboBox::textColourId, textWhite);
    setColour(juce::TextEditor::textColourId, textWhite);
    setColour(juce::TextEditor::backgroundColourId, bgDark);
    setColour(juce::TextEditor::outlineColourId, neonCyan);
    setColour(juce::Label::textColourId, textWhite);
}

void LookAndFeelNeon::drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h,
                                       float sliderPos, float rotaryStartAngle,
                                       float rotaryEndAngle, juce::Slider&)
{
    float radius = juce::jmin(static_cast<float>(w), static_cast<float>(h)) * 0.5f;
    float centre = radius + 0.5f;
    float rx = x + centre;
    float ry = y + centre;

    // Outer glow ring
    g.setColour(neonCyan.withMultipliedAlpha(0.3f));
    g.drawEllipse(rx - radius - 2, ry - radius - 2,
                  (radius + 2) * 2, (radius + 2) * 2, 3.0f);

    // Track ring
    g.setColour(juce::Colour(0x3000f0ff));
    g.drawEllipse(rx - radius, ry - radius, radius * 2, radius * 2, 2.0f);

    // Filled arc
    float startAngle = rotaryStartAngle;
    float endAngle = rotaryStartAngle + (rotaryEndAngle - rotaryStartAngle) * sliderPos;

    g.setColour(neonCyan);
    g.fillPath([](float sa, float ea, float r, float cx, float cy) {
        juce::Path p;
        p.addCentredArc(cx, cy, r * 0.85f, r * 0.85f, 0, sa, ea, true);
        return p;
    }(startAngle, endAngle, radius, rx, ry));

    // Thumb knob
    float thumbAngle = startAngle + (endAngle - startAngle) * sliderPos;
    float thumbX = rx + std::cos(thumbAngle - MathConstants<float>::halfPi) * radius * 0.7f;
    float thumbY = ry + std::sin(thumbAngle - MathConstants<float>::halfPi) * radius * 0.7f;

    g.setColour(neonCyan);
    g.fillEllipse(thumbX - 5, thumbY - 5, 10, 10);

    // Glow on thumb
    g.setColour(neonCyan.withMultipliedAlpha(0.5f));
    g.fillEllipse(thumbX - 8, thumbY - 8, 16, 16);
}

void LookAndFeelNeon::drawButtonText(juce::Graphics& g, juce::Button& button,
                                     bool isMouseOver, bool isMouseDown)
{
    auto bounds = button.getLocalBounds();
    g.setColour(button.findAndApplyColour(juce::TextButton::buttonColourId));
    g.fillAll();

    if (isMouseOver || isMouseDown)
    {
        g.setColour(neonCyan.withMultipliedAlpha(0.15f));
        g.fillAll();
    }

    g.setColour(textWhite);
    g.setFont(juce::Font(13.0f, juce::Font::medium));
    g.drawFittedText(button.getButtonText(), bounds,
                     juce::Justification::centred, 1);
}

void LookAndFeelNeon::drawComboBox(juce::Graphics& g, int w, int h, bool,
                                   int, int, int, int, juce::ComboBox& box)
{
    g.setColour(bgDark);
    g.fillAll();

    g.setColour(neonCyan);
    g.drawRect(0, 0, w, h, 1.5f);

    if (!box.getText().isEmpty())
    {
        g.setColour(textWhite);
        g.setFont(juce::Font(12.0f, juce::Font::plain));
        g.drawText(box.getText(), 6, 0, w - 30, h,
                   juce::Justification::centredLeft, true);
    }
}

} // namespace NeonSynth
