/*
  NeonSynth - PluginEditor
  Main UI with neon theme — JUCE 8.0.14 compatible
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace NeonSynth
{
    PluginEditor::PluginEditor(PluginProcessor& p)
        : AudioProcessorEditor(p),
          processor_(p)
    {
        setLookAndFeel(&lookAndFeel_);
        setSize(500, 600);

        // --- Oscillator 1 wave ---
        osc1Wave_.addItem("Sine", 1);
        osc1Wave_.addItem("Saw", 2);
        osc1Wave_.addItem("Square", 3);
        osc1Wave_.addItem("Triangle", 4);
        addAndMakeVisible(osc1Wave_);
        osc1WaveLabel_.setText("OSC1 Wave", juce::dontSendNotification);
        osc1WaveLabel_.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
        addAndMakeVisible(osc1WaveLabel_);

        // --- Oscillator 2 wave ---
        osc2Wave_.addItem("Sine", 1);
        osc2Wave_.addItem("Saw", 2);
        osc2Wave_.addItem("Square", 3);
        osc2Wave_.addItem("Triangle", 4);
        addAndMakeVisible(osc2Wave_);
        osc2WaveLabel_.setText("OSC2 Wave", juce::dontSendNotification);
        osc2WaveLabel_.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
        addAndMakeVisible(osc2WaveLabel_);

        // --- Filter type ---
        filterType_.addItem("Low Pass", 1);
        filterType_.addItem("High Pass", 2);
        filterType_.addItem("Band Pass", 3);
        addAndMakeVisible(filterType_);
        filterTypeLabel_.setText("Filter Type", juce::dontSendNotification);
        filterTypeLabel_.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
        addAndMakeVisible(filterTypeLabel_);

        // --- Detune sliders (horizontal with text box) ---
        osc1Detune_.setRange(-50.0, 50.0, 1.0);
        osc1Detune_.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        osc1Detune_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 0);
        osc1DetuneLabel_.setText("Detune", juce::dontSendNotification);
        osc1DetuneLabel_.attachToComponent(&osc1Detune_, true);
        addAndMakeVisible(osc1Detune_);
        addAndMakeVisible(osc1DetuneLabel_);

        osc1Gain_.setRange(0.0, 1.0, 0.01);
        osc1Gain_.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        osc1Gain_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 0);
        osc1GainLabel_.setText("Gain", juce::dontSendNotification);
        osc1GainLabel_.attachToComponent(&osc1Gain_, true);
        addAndMakeVisible(osc1Gain_);
        addAndMakeVisible(osc1GainLabel_);

        osc2Detune_.setRange(-50.0, 50.0, 1.0);
        osc2Detune_.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        osc2Detune_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 0);
        osc2DetuneLabel_.setText("Detune", juce::dontSendNotification);
        osc2DetuneLabel_.attachToComponent(&osc2Detune_, true);
        addAndMakeVisible(osc2Detune_);
        addAndMakeVisible(osc2DetuneLabel_);

        osc2Gain_.setRange(0.0, 1.0, 0.01);
        osc2Gain_.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        osc2Gain_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 0);
        osc2GainLabel_.setText("Gain", juce::dontSendNotification);
        osc2GainLabel_.attachToComponent(&osc2Gain_, true);
        addAndMakeVisible(osc2Gain_);
        addAndMakeVisible(osc2GainLabel_);

        // --- Filter sliders (vertical bars with labels above) ---
        filterCutoff_.setRange(20.0, 20000.0, 1.0);
        filterCutoff_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        filterCutoff_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        filterCutoffLabel_.setText("Cutoff", juce::dontSendNotification);
        filterCutoffLabel_.attachToComponent(&filterCutoff_, true);
        addAndMakeVisible(filterCutoff_);
        addAndMakeVisible(filterCutoffLabel_);

        filterRes_.setRange(0.0, 0.9, 0.01);
        filterRes_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        filterRes_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        filterResLabel_.setText("Resonance", juce::dontSendNotification);
        filterResLabel_.attachToComponent(&filterRes_, true);
        addAndMakeVisible(filterRes_);
        addAndMakeVisible(filterResLabel_);

        // --- Amp envelope ---
        ampAttack_.setRange(0.001, 5.0, 0.001);
        ampAttack_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        ampAttack_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        ampAttackLabel_.setText("Amp Atk", juce::dontSendNotification);
        ampAttackLabel_.attachToComponent(&ampAttack_, true);
        addAndMakeVisible(ampAttack_);
        addAndMakeVisible(ampAttackLabel_);

        ampDecay_.setRange(0.001, 5.0, 0.001);
        ampDecay_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        ampDecay_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        ampDecayLabel_.setText("Amp Dec", juce::dontSendNotification);
        ampDecayLabel_.attachToComponent(&ampDecay_, true);
        addAndMakeVisible(ampDecay_);
        addAndMakeVisible(ampDecayLabel_);

        ampSustain_.setRange(0.0, 1.0, 0.01);
        ampSustain_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        ampSustain_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        ampSustainLabel_.setText("Amp Sus", juce::dontSendNotification);
        ampSustainLabel_.attachToComponent(&ampSustain_, true);
        addAndMakeVisible(ampSustain_);
        addAndMakeVisible(ampSustainLabel_);

        ampRelease_.setRange(0.001, 10.0, 0.001);
        ampRelease_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        ampRelease_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        ampReleaseLabel_.setText("Amp Rel", juce::dontSendNotification);
        ampReleaseLabel_.attachToComponent(&ampRelease_, true);
        addAndMakeVisible(ampRelease_);
        addAndMakeVisible(ampReleaseLabel_);

        // --- Filter envelope ---
        filterAttack_.setRange(0.001, 5.0, 0.001);
        filterAttack_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        filterAttack_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        filterAttackLabel_.setText("Flt Atk", juce::dontSendNotification);
        filterAttackLabel_.attachToComponent(&filterAttack_, true);
        addAndMakeVisible(filterAttack_);
        addAndMakeVisible(filterAttackLabel_);

        filterDecay_.setRange(0.001, 5.0, 0.001);
        filterDecay_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        filterDecay_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        filterDecayLabel_.setText("Flt Dec", juce::dontSendNotification);
        filterDecayLabel_.attachToComponent(&filterDecay_, true);
        addAndMakeVisible(filterDecay_);
        addAndMakeVisible(filterDecayLabel_);

        filterSustain_.setRange(0.0, 1.0, 0.01);
        filterSustain_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        filterSustain_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        filterSustainLabel_.setText("Flt Sus", juce::dontSendNotification);
        filterSustainLabel_.attachToComponent(&filterSustain_, true);
        addAndMakeVisible(filterSustain_);
        addAndMakeVisible(filterSustainLabel_);

        filterRelease_.setRange(0.001, 10.0, 0.001);
        filterRelease_.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        filterRelease_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 0);
        filterReleaseLabel_.setText("Flt Rel", juce::dontSendNotification);
        filterReleaseLabel_.attachToComponent(&filterRelease_, true);
        addAndMakeVisible(filterRelease_);
        addAndMakeVisible(filterReleaseLabel_);

        // --- Wire up APVTS attachments ---
        attachControls();
    }

    void PluginEditor::attachControls()
    {
        auto& apvts = processor_.getAPVTS();

        osc1WaveAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, "OSC1_WAVE", osc1Wave_);
        osc2WaveAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, "OSC2_WAVE", osc2Wave_);
        osc1DetuneAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "OSC1_DETUNE", osc1Detune_);
        osc1GainAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "OSC1_GAIN", osc1Gain_);
        osc2DetuneAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "OSC2_DETUNE", osc2Detune_);
        osc2GainAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "OSC2_GAIN", osc2Gain_);
        filterTypeAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, "FILTER_TYPE", filterType_);
        filterCutoffAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "FILTER_CUTOFF", filterCutoff_);
        filterResAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "FILTER_RES", filterRes_);
        ampAttackAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "AMP_ATTACK", ampAttack_);
        ampDecayAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "AMP_DECAY", ampDecay_);
        ampSustainAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "AMP_SUSTAIN", ampSustain_);
        ampReleaseAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "AMP_RELEASE", ampRelease_);
        filterAttackAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "FILTER_ATTACK", filterAttack_);
        filterDecayAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "FILTER_DECAY", filterDecay_);
        filterSustainAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "FILTER_SUSTAIN", filterSustain_);
        filterReleaseAPVTSAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, "FILTER_RELEASE", filterRelease_);
    }

    PluginEditor::~PluginEditor()
    {
        setLookAndFeel(nullptr);
    }

    void PluginEditor::paint(juce::Graphics& g)
    {
        g.fillAll(NeonSynth::LookAndFeelNeon::bgDark());
    }

    void PluginEditor::resized()
    {
        auto bounds = getLocalBounds();
        bounds.removeFromTop(10);

        // Top row: OSC1 + OSC2 wave selectors
        auto oscRow = bounds.removeFromTop(40);
        oscRow.removeFromLeft(10);
        oscRow.removeFromRight(10);
        auto osc1Area = oscRow.removeFromLeft(oscRow.getWidth() / 2);
        osc1WaveLabel_.setBounds(osc1Area.removeFromTop(18));
        osc1Wave_.setBounds(osc1Area.removeFromTop(22));
        auto osc2Area = oscRow.removeFromRight(osc1Area.getWidth());
        osc2WaveLabel_.setBounds(osc2Area.removeFromTop(18));
        osc2Wave_.setBounds(osc2Area.removeFromTop(22));

        bounds.removeFromTop(10);

        // Detune + gain sliders
        auto paramRow = bounds.removeFromTop(80);
        paramRow.removeFromLeft(10);
        paramRow.removeFromRight(10);
        auto osc1Params = paramRow.removeFromLeft(paramRow.getWidth() / 2);
        osc1Detune_.setBounds(osc1Params.removeFromTop(30));
        osc1Gain_.setBounds(osc1Params.removeFromTop(30));
        auto osc2Params = paramRow.removeFromRight(osc1Params.getWidth());
        osc2Detune_.setBounds(osc2Params.removeFromTop(30));
        osc2Gain_.setBounds(osc2Params.removeFromTop(30));

        bounds.removeFromTop(15);

        // Filter section
        auto filterSection = bounds.removeFromTop(140);
        filterSection.removeFromLeft(10);
        filterSection.removeFromRight(10);
        filterTypeLabel_.setBounds(filterSection.removeFromTop(18));
        filterType_.setBounds(filterSection.removeFromTop(22));
        auto filterParams = filterSection.reduced(20, 0);
        filterCutoff_.setBounds(filterParams.removeFromLeft(filterParams.getWidth() / 2));
        filterRes_.setBounds(filterParams);

        bounds.removeFromTop(20);

        // Envelope section
        auto envSection = bounds.reduced(10, 10);
        auto ampEnv = envSection.removeFromLeft(envSection.getWidth() / 2);
        auto filterEnv = envSection;

        int sliderH = ampEnv.getHeight() / 4;
        ampAttack_.setBounds(ampEnv.removeFromTop(sliderH));
        ampDecay_.setBounds(ampEnv.removeFromTop(sliderH));
        ampSustain_.setBounds(ampEnv.removeFromTop(sliderH));
        ampRelease_.setBounds(ampEnv.removeFromTop(sliderH));

        sliderH = filterEnv.getHeight() / 4;
        filterAttack_.setBounds(filterEnv.removeFromTop(sliderH));
        filterDecay_.setBounds(filterEnv.removeFromTop(sliderH));
        filterSustain_.setBounds(filterEnv.removeFromTop(sliderH));
        filterRelease_.setBounds(filterEnv.removeFromTop(sliderH));
    }
}
