/*
  NeonSynth - Envelope Editor Implementation
*/

#include "UI/EnvelopeEditor.h"

namespace NeonSynth {

EnvelopeEditor::EnvelopeEditor(const juce::String& t)
    : title_(t)
{
    setOpaque(true);
}

EnvelopeEditor::~EnvelopeEditor() = default;

void EnvelopeEditor::setAttack(double s) { attack_ = s; repaint(); }
void EnvelopeEditor::setDecay(double s) { decay_ = s; repaint(); }
void EnvelopeEditor::setSustain(double l) { sustain_ = l; repaint(); }
void EnvelopeEditor::setRelease(double s) { release_ = s; repaint(); }

void EnvelopeEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff08080a));

    int w = getWidth();
    int h = getHeight();
    int margin = 20;

    // Title
    g.setColour(juce::Colour(0xff00f0ff));
    g.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
    g.drawText(title_, margin, 5, 100, 16, juce::Justification::centredLeft);

    // Draw ADSR envelope shape
    double total = attack_ + decay_ + 1.0 + release_; // 1.0 = sustain hold
    double aEnd = attack_ / total * (w - 2 * margin) + margin;
    double dEnd = aEnd + (decay_ / total) * (w - 2 * margin);
    double sEnd = dEnd; // sustain holds level
    double rEnd = w - margin;
    double sY = margin + (1.0 - sustain_) * (h - 2 * margin);

    // Envelope path
    juce::Path env;
    env.startNewSubPath(margin, h - margin);           // start bottom-left
    env.lineTo(aEnd, margin);                          // attack peak
    env.lineTo(dEnd, sY);                              // decay to sustain
    env.lineTo(sEnd, sY);                              // sustain hold
    env.lineTo(rEnd, h - margin);                      // release to zero

    // Glow
    g.setColour(juce::Colour(0x3000f0ff));
    g.strokePath(env, juce::PathStrokeType(8.0f));

    // Main line
    g.setColour(juce::Colour(0xff00f0ff));
    g.strokePath(env, juce::PathStrokeType(2.0f));

    // Fill under curve
    env.lineTo(rEnd, h - margin);
    env.lineTo(margin, h - margin);
    g.setColour(juce::Colour(0x1000f0ff));
    g.fillPath(env);

    // Phase labels
    g.setColour(juce::Colour(0xff666670));
    g.setFont(juce::Font(juce::FontOptions(9.0f)));
    g.drawText("A", margin + 5, h - 15, 15, 12, juce::Justification::centred);
    g.drawText("D", static_cast<int>(aEnd), h - 15, 15, 12, juce::Justification::centred);
    g.drawText("S", static_cast<int>(dEnd), h - 15, 15, 12, juce::Justification::centred);
    g.drawText("R", static_cast<int>(sEnd) + 10, h - 15, 15, 12, juce::Justification::centred);
}

void EnvelopeEditor::mouseDown(const juce::MouseEvent&) {}
void EnvelopeEditor::mouseDrag(const juce::MouseEvent&) {}

} // namespace NeonSynth
