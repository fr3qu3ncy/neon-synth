/*
  NeonSynth - Filter Editor Implementation
*/

#include "UI/FilterEditor.h"

namespace NeonSynth {

FilterEditor::FilterEditor()
{
    setOpaque(true);
}

FilterEditor::~FilterEditor() = default;

void FilterEditor::setCutoff(double hz) { cutoff_ = hz; repaint(); }
void FilterEditor::setResonance(double r) { resonance_ = r; repaint(); }
void FilterEditor::setType(int t) { filterType_ = t; repaint(); }

double FilterEditor::getResponse(double freq) const
{
    double ratio = freq / cutoff_;
    double q = 1.0 - resonance_;

    if (filterType_ == 0) // LP
        return 1.0 / std::sqrt(std::pow(1.0 - ratio * ratio, 2) + std::pow(ratio / q, 2));
    if (filterType_ == 1) // HP
        return ratio * ratio / std::sqrt(std::pow(1.0 - ratio * ratio, 2) + std::pow(ratio / q, 2));
    // BP
    return (ratio / q) / std::sqrt(std::pow(1.0 - ratio * ratio, 2) + std::pow(ratio / q, 2));
}

void FilterEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff08080a));

    int w = getWidth();
    int h = getHeight();
    int margin = 20;

    // Title
    g.setColour(juce::Colour(0xff4060ff));
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText("FILTER RESPONSE", margin, 5, 150, 16, juce::Justification::centredLeft);

    // Frequency axis (logarithmic, 20Hz - 20kHz)
    double logMin = std::log(20.0);
    double logMax = std::log(20000.0);

    // Grid
    g.setColour(juce::Colour(0x154060ff));
    for (int f : {50, 100, 200, 500, 1000, 2000, 5000, 10000})
    {
        double x = margin + (std::log(static_cast<double>(f)) - logMin) / (logMax - logMin)
                   * (w - 2 * margin);
        g.drawLine(static_cast<int>(x), margin, static_cast<int>(x), h - margin, 1.0f);

        g.setColour(juce::Colour(0xff666670));
        g.setFont(juce::Font(8.0f));
        g.drawText(juce::String(f) + "Hz", static_cast<int>(x) - 10, h - 12, 30, 10);
        g.setColour(juce::Colour(0x154060ff));
    }

    // Response curve
    juce::Path curve;
    bool first = true;

    for (int x = margin; x < w - margin; ++x)
    {
        double logFreq = logMin + (static_cast<double>(x - margin) / (w - 2 * margin))
                         * (logMax - logMin);
        double freq = std::exp(logFreq);
        double mag = getResponse(freq);

        int y = h - margin - static_cast<int>(mag * (h - 2 * margin));

        if (first)
        {
            curve.startNewSubPath(x, y);
            first = false;
        }
        else
        {
            curve.lineTo(x, y);
        }
    }

    // Glow
    g.setColour(juce::Colour(0x304060ff));
    g.strokePath(curve, juce::PathStrokeType(8.0f));

    // Main line
    g.setColour(juce::Colour(0xff4060ff));
    g.strokePath(curve, juce::PathStrokeType(2.0f));

    // Cutoff marker
    double cutX = margin + (std::log(cutoff_) - logMin) / (logMax - logMin) * (w - 2 * margin);
    g.setColour(juce::Colour(0xffff00aa));
    g.drawLine(static_cast<int>(cutX), margin, static_cast<int>(cutX), h - margin, 2.0f);
}

} // namespace NeonSynth
