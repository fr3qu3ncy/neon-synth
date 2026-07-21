/*
  NeonSynth - Oscilloscope Implementation
*/

#include "UI/Oscilloscope.h"

namespace NeonSynth {

Oscilloscope::Oscilloscope()
{
    setOpaque(true);
}

Oscilloscope::~Oscilloscope() = default;

void Oscilloscope::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff08080a));

    auto lock = juce::ScopedLock(bufferLock_);

    int w = getWidth();
    int h = getHeight();
    int midY = h / 2;

    // Grid lines
    g.setColour(juce::Colour(0x1500f0ff));
    for (int i = 1; i < 4; ++i)
    {
        int y = midY - (h / 4) * i;
        g.drawLine(0, y, w, y, 1.0f);
        y = midY + (h / 4) * i;
        g.drawLine(0, y, w, y, 1.0f);
    }

    // Center line
    g.setColour(juce::Colour(0x3000f0ff));
    g.drawLine(0, midY, w, midY, 1.0f);

    // OSC1 waveform - cyan
    if (!osc1Buffer_.empty())
    {
        g.setColour(juce::Colour(0xff00f0ff));
        g.setStrokeType(1.0f, 2.0f);

        juce::Path p1;
        for (size_t i = 0; i < osc1Buffer_.size(); ++i)
        {
            float x = static_cast<float>(i) / osc1Buffer_.size() * w;
            float y = midY - osc1Buffer_[i] * (midY * 0.8f);
            if (i == 0) p1.startNewSubPath(x, y);
            else p1.lineTo(x, y);
        }
        g.strokePath(p1, juce::PathStrokeType(2.0f));

        // Glow layer
        g.setColour(juce::Colour(0x4000f0ff));
        g.strokePath(p1, juce::PathStrokeType(6.0f));
    }

    // OSC2 waveform - magenta
    if (!osc2Buffer_.empty())
    {
        g.setColour(juce::Colour(0xffff00aa));
        g.setStrokeType(1.0f, 2.0f);

        juce::Path p2;
        for (size_t i = 0; i < osc2Buffer_.size(); ++i)
        {
            float x = static_cast<float>(i) / osc2Buffer_.size() * w;
            float y = midY - osc2Buffer_[i] * (midY * 0.8f);
            if (i == 0) p2.startNewSubPath(x, y);
            else p2.lineTo(x, y);
        }
        g.strokePath(p2, juce::PathStrokeType(2.0f));

        // Glow layer
        g.setColour(juce::Colour(0x40ff00aa));
        g.strokePath(p2, juce::PathStrokeType(6.0f));
    }

    // Labels
    g.setColour(juce::Colour(0xff00f0ff));
    g.setFont(juce::Font(10.0f));
    g.drawText("OSC1", 5, 5, 40, 15, juce::Justification::centredLeft);

    g.setColour(juce::Colour(0xffff00aa));
    g.drawText("OSC2", 5, 20, 40, 15, juce::Justification::centredLeft);
}

void Oscilloscope::update(const std::vector<float>& osc1, const std::vector<float>& osc2)
{
    auto lock = juce::ScopedLock(bufferLock_);
    osc1Buffer_ = osc1;
    osc2Buffer_ = osc2;
    repaint();
}

} // namespace NeonSynth
