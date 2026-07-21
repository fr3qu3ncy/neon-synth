/*
  NeonSynth - PluginProcessor Implementation
*/

#include "PluginProcessor.h"

namespace NeonSynth {

PluginProcessor::PluginProcessor()
    : AudioProcessor(juce::AudioProcessor::BusesProperties()
          .withInput("Input", juce::AudioChannelSet::disabled(), false)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    // ---- Oscillator 1 parameters ----
    paramOsc1Waveform = &getParameters()->createParameter<float>(
        "osc1wf", "OSC1 Waveform",
        juce::NormalisableRange<float>(0.f, 3.99f, 1.f), 0.f,
        [](float v, int s) { return juce::String({"Sine", "Saw", "Square", "Triangle"})[static_cast<int>(v)]; },
        [](const juce::String& t) {
            static const juce::StringArray w{"Sine", "Saw", "Square", "Triangle"};
            return static_cast<float>(w.indexOf(t));
        }, juce::AudioProcessorParameter::genericParameter);

    paramOsc1Detune = &getParameters()->createParameter<float>(
        "osc1dt", "OSC1 Detune", juce::NormalisableRange<float>(-50.f, 50.f), 0.f,
        juce::AudioProcessorParameter::floatToString, juce::AudioProcessorParameter::stringToFloat);

    paramOsc1Gain = &getParameters()->createParameter<float>(
        "osc1gn", "OSC1 Gain", juce::NormalisableRange<float>(0.f, 1.f), 0.5f);

    paramOsc1Pan = &getParameters()->createParameter<float>(
        "osc1pn", "OSC1 Pan", juce::NormalisableRange<float>(-1.f, 1.f), 0.f);

    // ---- Oscillator 2 parameters ----
    paramOsc2Waveform = &getParameters()->createParameter<float>(
        "osc2wf", "OSC2 Waveform",
        juce::NormalisableRange<float>(0.f, 3.99f, 1.f), 1.f,
        [](float v, int s) { return juce::String({"Sine", "Saw", "Square", "Triangle"})[static_cast<int>(v)]; },
        [](const juce::String& t) {
            static const juce::StringArray w{"Sine", "Saw", "Square", "Triangle"};
            return static_cast<float>(w.indexOf(t));
        }, juce::AudioProcessorParameter::genericParameter);

    paramOsc2Detune = &getParameters()->createParameter<float>(
        "osc2dt", "OSC2 Detune", juce::NormalisableRange<float>(-50.f, 50.f), 0.f);

    paramOsc2Gain = &getParameters()->createParameter<float>(
        "osc2gn", "OSC2 Gain", juce::NormalisableRange<float>(0.f, 1.f), 0.5f);

    paramOsc2Pan = &getParameters()->createParameter<float>(
        "osc2pn", "OSC2 Pan", juce::NormalisableRange<float>(-1.f, 1.f), 0.f);

    // ---- Amplitude Envelope ----
    paramAmpEnvAttack = &getParameters()->createParameter<float>(
        "aenvat", "Amp Attack", juce::NormalisableRange<float>(0.001f, 5.f), 0.01f);
    paramAmpEnvDecay = &getParameters()->createParameter<float>(
        "aenvdc", "Amp Decay", juce::NormalisableRange<float>(0.001f, 5.f), 0.3f);
    paramAmpEnvSustain = &getParameters()->createParameter<float>(
        "aenvsu", "Amp Sustain", juce::NormalisableRange<float>(0.f, 1.f), 0.7f);
    paramAmpEnvRelease = &getParameters()->createParameter<float>(
        "aenvrl", "Amp Release", juce::NormalisableRange<float>(0.001f, 10.f), 0.5f);

    // ---- Filter Envelope ----
    paramFilterEnvAttack = &getParameters()->createParameter<float>(
        "fenvat", "Filter Env Attack", juce::NormalisableRange<float>(0.001f, 5.f), 0.01f);
    paramFilterEnvDecay = &getParameters()->createParameter<float>(
        "fenvdc", "Filter Env Decay", juce::NormalisableRange<float>(0.001f, 5.f), 0.3f);
    paramFilterEnvSustain = &getParameters()->createParameter<float>(
        "fenvsu", "Filter Env Sustain", juce::NormalisableRange<float>(0.f, 1.f), 0.5f);
    paramFilterEnvRelease = &getParameters()->createParameter<float>(
        "fenvrl", "Filter Env Release", juce::NormalisableRange<float>(0.001f, 10.f), 0.5f);

    // ---- Filter ----
    paramFilterType = &getParameters()->createParameter<float>(
        "fltyp", "Filter Type",
        juce::NormalisableRange<float>(0.f, 1.99f, 1.f), 0.f,
        [](float v, int s) { return juce::String({"LP", "HP", "BP"})[static_cast<int>(v)]; },
        [](const juce::String& t) {
            static const juce::StringArray f{"LP", "HP", "BP"};
            return static_cast<float>(f.indexOf(t));
        }, juce::AudioProcessorParameter::genericParameter);

    paramFilterCutoff = &getParameters()->createParameter<float>(
        "flcut", "Filter Cutoff", juce::NormalisableRange<float>(20.f, 20000.f), 1000.f);
    paramFilterResonance = &getParameters()->createParameter<float>(
        "flres", "Filter Resonance", juce::NormalisableRange<float>(0.f, 0.9f), 0.0f);
}

PluginProcessor::~PluginProcessor() = default;

void PluginProcessor::prepare(const juce::AudioProcessorPrepareInfo& info)
{
    if (!initialized_)
    {
        voiceManager_.setSampleRate(info.sampleRate);
        initialized_ = true;
    }
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;

    // Process MIDI events
    midi.removeAllEventsUntilNextActiveSensing();

    for (int i = 0; i < buffer.getNumChannels(); ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Process MIDI notes
    juce::MidiMessageSequence messages(midi);
    for (const auto metadata : messages)
    {
        const auto& message = metadata.message;
        int samplePos = static_cast<int>(metadata.samplePosition);

        if (message.isNoteOn())
            voiceManager_.noteOn(message.getNote(), message.getVelocityScaled0To1());
        else if (message.isNoteOff())
            voiceManager_.noteOff(message.getNote());
    }

    // Process audio
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        double outL = 0.0, outR = 0.0;
        voiceManager_.processSample(outL, outR);

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.setSample(ch, sample, static_cast<float>(ch == 0 ? outL : outR));
    }
}

void PluginProcessor::reset()
{
    initialized_ = false;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new juce::AudioProcessorEditor(*this); // Placeholder - replaced by PluginEditor
}

juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

} // namespace NeonSynth
