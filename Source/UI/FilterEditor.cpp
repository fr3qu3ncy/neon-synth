/*
  NeonSynth - Filter Editor Panel
  Filter type selection, cutoff, resonance
  JUCE 8 compatible
*/

#include "PluginProcessor.h"
#include "UI/LookAndFeelNeon.h"

namespace NeonSynth {

class FilterEditor : public juce::Component
{
public:
    FilterEditor(PluginProcessor& p)
        : processor(p),
          filterType("Filter Type"),
          cutoffSlider("Cutoff"),
          resSlider("Resonance")
    {
        filterType.addItem("LP", 1);
        filterType.addItem("HP", 2);
        filterType.addItem("BP", 3);
        filterType.setSelectedId(1);

        cutoffSlider.setRange(20.0, 20000.0);
        cutoffSlider.setValue(1000.0);
        cutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 80, 20);

        resSlider.setRange(0.0, 0.9);
        resSlider.setValue(0.0);
        resSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 80, 20);

        addAndMakeVisible(filterType);
        addAndMakeVisible(cutoffSlider);
        addAndMakeVisible(resSlider);
    }

    ~FilterEditor() override = default;

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xff08080a));

        // Title
        g.setColour(juce::Colour(0xff00ffff));
        g.setFont(juce::Font(juce::FontOptions(14.0f, juce::Font::bold)));
        g.drawText("FILTER", 10, 5, 100, 20, juce::Justification::centredLeft);

        // Draw filter curve visualization
        int w = getWidth();
        int h = getHeight();
        int curveH = h - 60;

        g.setColour(juce::Colour(0x154060ff));
        g.drawRect(10, 30, w - 20, curveH, 1);

        // Simple magnitude response curve
        juce::Path path;
        bool first = true;
        for (int x = 0; x < w - 20; ++x)
        {
            double freq = 20.0 + (double)x / (w - 20) * 19980.0;
            double mag = getCutoff() > 0 ? std::min(1.0, freq / getCutoff()) : 0.0;
            float px = 10 + x;
            float py = 30 + curveH - (float)mag * (curveH - 10);

            if (first)
            {
                path.startNewSubPath(px, py);
                first = false;
            }
            else
            {
                path.lineTo(px, py);
            }
        }

        g.setColour(juce::Colour(0xff4060ff));
        g.strokePath(path, juce::PathStrokeType(2.0f));
    }

    void resized() override
    {
        int w = getWidth();
        int h = getHeight();

        filterType.setBounds(10, h - 30, 80, 25);
        cutoffSlider.setBounds(100, h - 50, w - 120, 20);
        resSlider.setBounds(100, h - 25, w - 120, 20);
    }

private:
    PluginProcessor& processor;
    juce::ComboBox filterType;
    juce::Slider cutoffSlider;
    juce::Slider resSlider;

    double getCutoff() const { return cutoffSlider.getValue(); }
};

} // namespace NeonSynth
