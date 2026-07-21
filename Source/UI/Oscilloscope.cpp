/*
  NeonSynth - Oscilloscope Implementation
  JUCE 8 compatible
*/

#include "UI/Oscilloscope.h"

namespace NeonSynth {

Oscilloscope::Oscilloscope()
{
    waveform_.resize(1024, 0.0f);
}

Oscilloscope::~Oscilloscope() = default;

void Oscilloscope::update(const float* data, int numSamples)
{
    juce::ScopedLock lock(waveLock_);
    for (int i = 0; i < numSamples && i < (int)waveform_.size(); ++i)
        waveform_[i] = data[i];
}

void Oscilloscope::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff08080a));

    int w = getWidth();
    int h = getHeight();
    int margin = 10;

    {
        juce::ScopedLock lock(waveLock_);

        if (waveform_.empty())
            return;

        // Center line
        g.setColour(juce::Colour(0x154060ff));
        g.drawLine(margin, h / 2, w - margin, h / 2, 1.0f);

        // Waveform path
        juce::Path path;
        bool first = true;

        const int N = (int)waveform_.size();
        for (int i = 0; i < N && i < w - 2 * margin; ++i)
        {
            float x = margin + (float)i / N * (w - 2 * margin);
            float y = h / 2 - waveform_[i] * (h / 2 - margin);

            if (first)
            {
                path.startNewSubPath(x, y);
                first = false;
            }
            else
            {
                path.lineTo(x, y);
            }
        }

        // Glow
        g.setColour(juce::Colour(0x3000ffff));
        g.strokePath(path, juce::PathStrokeType(6.0f));

        // Main line
        g.setColour(juce::Colour(0xff00ffff));
        g.strokePath(path, juce::PathStrokeType(2.0f));
    }

    // Label
    g.setColour(juce::Colour(0xff666670));
    g.setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::plain)));
    g.drawText("OSCILLOSCOPE", 2, 2, 100, 14, juce::Justification::centredLeft);
}

} // namespace NeonSynth
