/*
  NeonSynth - Audio Processor Implementation
  JUCE 8.0.14 compatible (old API: prepareToPlay/processBlock)
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace NeonSynth {

//==============================================================================
PluginProcessor::PluginProcessor()
    : AudioProcessor(juce::AudioProcessor::BusesProperties()
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts_(*this, nullptr, "PARAMS", juce::AudioProcessorValueTreeState::ParameterLayout{})
{
    // Oscillator 1
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{"osc1wave", 1}, "Osc1 Wave", 0, 3, 0));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"osc1pitch", 1}, "Osc1 Pitch",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 1.0f), 0.0f));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"osc1detune", 1}, "Osc1 Detune",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));

    // Oscillator 2
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{"osc2wave", 1}, "Osc2 Wave", 0, 3, 0));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"osc2pitch", 1}, "Osc2 Pitch",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 1.0f), 0.0f));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"osc2detune", 1}, "Osc2 Detune",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"osc2mix", 1}, "Osc2 Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    // Filter
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"fcutoff", 1}, "Cutoff",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.1f), 5000.0f));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"fres", 1}, "Resonance",
        juce::NormalisableRange<float>(0.0f, 0.95f, 0.01f), 0.0f));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{"ftype", 1}, "Filter Type", 0, 2, 0));

    // Envelope
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"eattack", 1}, "Attack",
        juce::NormalisableRange<float>(0.001f, 2.0f, 0.001f, 0.1f), 0.01f));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"edecay", 1}, "Decay",
        juce::NormalisableRange<float>(0.01f, 2.0f, 0.01f, 0.1f), 0.3f));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"esustain", 1}, "Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.7f));
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"erelease", 1}, "Release",
        juce::NormalisableRange<float>(0.01f, 3.0f, 0.01f, 0.1f), 0.5f));

    // Master
    apvts_.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"master", 1}, "Master Vol",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.7f));
}

PluginProcessor::~PluginProcessor() = default;

//==============================================================================
void PluginProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    voiceManager_.setSampleRate(sampleRate);
}

void PluginProcessor::reset()
{
    // Nothing to reset in this version
}

//==============================================================================
void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    const int numSamples = (int)buffer.getNumSamples();

    // Silence the output buffer first
    buffer.clear();

    // Handle MIDI events
    for (auto metadata : midi)
    {
        auto message = metadata.getMessage();
        if (message.isNoteOn())
            voiceManager_.noteOn(message.getNoteNumber(), message.getVelocity() / 127.0f);
        else if (message.isNoteOff())
            voiceManager_.noteOff(message.getNoteNumber());
    }
    midi.clear();

    // Process audio
    const int numChannels = (int)buffer.getNumChannels();
    float* outLeft = buffer.getWritePointer(0, 0);
    float* outRight = (numChannels > 1) ? buffer.getWritePointer(1, 0) : outLeft;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        double outL = 0.0;
        double outR = 0.0;
        voiceManager_.processSample(outL, outR);
        outLeft[sample] = (float)outL;
        outRight[sample] = (float)outR;
    }
}

//==============================================================================
void PluginProcessor::getStateInformation(juce::MemoryBlock& data)
{
    auto state = apvts_.copyState();
    auto xml = state.createXml();
    if (xml)
    {
        auto xmlData = xml->toString();
        data.replaceAll(xmlData.toRawUTF8(), (size_t)xmlData.length());
    }
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (!data || sizeInBytes <= 0)
        return;

    juce::String xmlString((const char*)data, sizeInBytes);
    auto xml = juce::parseXML(xmlString);
    if (xml)
    {
        auto tree = juce::ValueTree::fromXml(*xml);
        if (tree.isValid())
            apvts_.replaceState(tree);
    }
}

//==============================================================================
juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    editor_ = std::make_unique<PluginEditor>(*this);
    return editor_.get();
}

} // namespace NeonSynth

// Create the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeonSynth::PluginProcessor();
}
