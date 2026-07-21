/*
  NeonSynth - Voice Implementation
*/

#include "Voice.h"

namespace NeonSynth {

Voice::Voice() = default;

void Voice::setSampleRate(double rate)
{
    osc1_.setSampleRate(rate);
    osc2_.setSampleRate(rate);
    ampEnv_.setSampleRate(rate);
    filterEnv_.setSampleRate(rate);
    filter_.setSampleRate(rate);
}

void Voice::setOsc1Waveform(Oscillator::Waveform wf) { osc1_.setWaveform(wf); }
void Voice::setOsc1Detune(double cents) { osc1_.setDetune(cents); }
void Voice::setOsc1Gain(double gain) { osc1_.setGain(gain); }
void Voice::setOsc1Pan(double pan) { /* stored for processSample */ }

void Voice::setOsc2Waveform(Oscillator::Waveform wf) { osc2_.setWaveform(wf); }
void Voice::setOsc2Detune(double cents) { osc2_.setDetune(cents); }
void Voice::setOsc2Gain(double gain) { osc2_.setGain(gain); }
void Voice::setOsc2Pan(double pan) { /* stored for processSample */ }

void Voice::setAmpEnvAttack(double sec) { ampEnv_.setAttack(sec); }
void Voice::setAmpEnvDecay(double sec) { ampEnv_.setDecay(sec); }
void Voice::setAmpEnvSustain(double level) { ampEnv_.setSustain(level); }
void Voice::setAmpEnvRelease(double sec) { ampEnv_.setRelease(sec); }

void Voice::setFilterEnvAttack(double sec) { filterEnv_.setAttack(sec); }
void Voice::setFilterEnvDecay(double sec) { filterEnv_.setDecay(sec); }
void Voice::setFilterEnvSustain(double level) { filterEnv_.setSustain(level); }
void Voice::setFilterEnvRelease(double sec) { filterEnv_.setRelease(sec); }

void Voice::setFilterCutoff(double hertz) { filter_.setCutoff(hertz); }
void Voice::setFilterResonance(double res) { filter_.setResonance(res); }
void Voice::setFilterType(Filter::FilterType type) { filter_.setType(type); }

double Voice::midiToFreq(double note)
{
    return 440.0 * std::pow(2.0, (note - 69.0) / 12.0);
}

double Voice::panToGain(double pan, bool left)
{
    // Equal-power panning
    double angle = pan * MathConstants<double>::pi * 0.5;
    return left ? std::cos(angle) : std::sin(angle);
}

void Voice::noteOn(double note, double velocity)
{
    note_ = note;
    velocity_ = juce::jlimit(0.0, 1.0, velocity);

    double freq = midiToFreq(note);
    osc1_.setFrequency(freq);
    osc2_.setFrequency(freq);

    ampEnv_.noteOn();
    filterEnv_.noteOn();

    playing_ = true;
    releasing_ = false;
}

void Voice::noteOff()
{
    ampEnv_.noteOff();
    filterEnv_.noteOff();
    playing_ = false;
    releasing_ = true;
}

void Voice::processSample(double& outLeft, double& outRight)
{
    // Generate oscillator samples
    double osc1Sample = osc1_.processSample();
    double osc2Sample = osc2_.processSample();

    // Store for oscilloscope
    osc1Out_ = osc1Sample;
    osc2Out_ = osc2Sample;

    // Sum oscillators
    double mixed = (osc1Sample + osc2Sample) * 0.5;

    // Apply amplitude envelope
    double ampValue = ampEnv_.processSample();
    mixed *= ampValue * velocity_;

    // Apply filter envelope to cutoff (will be implemented as dynamic cutoff)
    double filterValue = filterEnv_.processSample();
    // Note: for simplicity, filter cutoff deviation is applied in VoiceManager

    // Apply filter
    double filtered = filter_.processSample(mixed);

    // Check if voice can be killed
    if (!playing_ && ampEnv_.getPhase() == Envelope::Phase::Off)
    {
        releasing_ = false;
        outLeft = 0.0;
        outRight = 0.0;
        return;
    }

    // Pan and output (simplified - pan values would be stored members)
    outLeft = filtered;
    outRight = filtered;
}

} // namespace NeonSynth
