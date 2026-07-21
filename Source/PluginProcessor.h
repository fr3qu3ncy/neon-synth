/*
  NeonSynth - PluginProcessor
  Main AudioProcessor with parameter tree and DSP routing
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/VoiceManager.h"
#include "DSP/Oscillator.h"
#include "DSP/Filter.h"

namespace NeonSynth {

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepare(const juce::AudioProcessorPrepareInfo& info) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override;
    void reset() override;

    juce::AudioProcessorEditor* createEditor() override;
    juce::String getName() const override;
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailSeconds() const override { return 2.0; }
    bool wantsSolidButtonBackground() const override { return true; }
    int getNumPrograms() override { return 0; }
    int getCurrentProgram() override { return 0; }
    void changeProgramName(int, const juce::String&) override {}
    void changeProgram(int) override {}
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

    /** Get the voice manager (for UI oscilloscope access) */
    VoiceManager& getVoiceManager() { return voiceManager_; }

    // ---- Parameters ----
    juce::RangedParameter<float>* paramOsc1Waveform;
    juce::RangedParameter<float>* paramOsc1Detune;
    juce::RangedParameter<float>* paramOsc1Gain;
    juce::RangedParameter<float>* paramOsc1Pan;
    juce::RangedParameter<float>* paramOsc2Waveform;
    juce::RangedParameter<float>* paramOsc2Detune;
    juce::RangedParameter<float>* paramOsc2Gain;
    juce::RangedParameter<float>* paramOsc2Pan;
    juce::RangedParameter<float>* paramAmpEnvAttack;
    juce::RangedParameter<float>* paramAmpEnvDecay;
    juce::RangedParameter<float>* paramAmpEnvSustain;
    juce::RangedParameter<float>* paramAmpEnvRelease;
    juce::RangedParameter<float>* paramFilterEnvAttack;
    juce::RangedParameter<float>* paramFilterEnvDecay;
    juce::RangedParameter<float>* paramFilterEnvSustain;
    juce::RangedParameter<float>* paramFilterEnvRelease;
    juce::RangedParameter<float>* paramFilterType;
    juce::RangedParameter<float>* paramFilterCutoff;
    juce::RangedParameter<float>* paramFilterResonance;

private:
    VoiceManager voiceManager_;
    bool initialized_ = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};

} // namespace NeonSynth
