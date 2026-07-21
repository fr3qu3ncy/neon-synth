/*
  NeonSynth - PluginEditor
  Main UI with neon theme — JUCE 8.0.14 compatible
*/

#pragma once

#include <JuceHeader.h>
#include "UI/LookAndFeelNeon.h"

namespace NeonSynth
{
    class PluginProcessor;

    class PluginEditor : public juce::AudioProcessorEditor
    {
    public:
        explicit PluginEditor(PluginProcessor&);
        ~PluginEditor() override;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        void attachControls();

        // Waveform selectors
        juce::ComboBox osc1Wave_;
        juce::ComboBox osc2Wave_;
        juce::ComboBox filterType_;
        juce::Label osc1WaveLabel_;
        juce::Label osc2WaveLabel_;
        juce::Label filterTypeLabel_;

        // Sliders
        juce::Slider osc1Detune_;
        juce::Slider osc1Gain_;
        juce::Slider osc2Detune_;
        juce::Slider osc2Gain_;
        juce::Slider filterCutoff_;
        juce::Slider filterRes_;
        juce::Slider ampAttack_;
        juce::Slider ampDecay_;
        juce::Slider ampSustain_;
        juce::Slider ampRelease_;
        juce::Slider filterAttack_;
        juce::Slider filterDecay_;
        juce::Slider filterSustain_;
        juce::Slider filterRelease_;

        // Slider labels (attached via attachToComponent)
        juce::Label osc1DetuneLabel_;
        juce::Label osc1GainLabel_;
        juce::Label osc2DetuneLabel_;
        juce::Label osc2GainLabel_;
        juce::Label filterCutoffLabel_;
        juce::Label filterResLabel_;
        juce::Label ampAttackLabel_;
        juce::Label ampDecayLabel_;
        juce::Label ampSustainLabel_;
        juce::Label ampReleaseLabel_;
        juce::Label filterAttackLabel_;
        juce::Label filterDecayLabel_;
        juce::Label filterSustainLabel_;
        juce::Label filterReleaseLabel_;

        // APVTS attachments
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1WaveAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2WaveAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1DetuneAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1GainAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2DetuneAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2GainAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampAttackAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampDecayAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampSustainAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampReleaseAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterAttackAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterDecayAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterSustainAPVTSAttach;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterReleaseAPVTSAttach;

        PluginProcessor& processor_;
        LookAndFeelNeon lookAndFeel_;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
    };
}
