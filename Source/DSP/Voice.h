/*
  NeonSynth - Single Polyphonic Voice
  Contains: 2 oscillators, 2 envelopes, 1 filter
*/

#pragma once

#include "Oscillator.h"
#include "Envelope.h"
#include "Filter.h"

namespace NeonSynth {

class Voice
{
public:
    Voice();
    ~Voice() = default;

    void setSampleRate(double rate);

    // Oscillator 1
    void setOsc1Waveform(Oscillator::Waveform wf);
    void setOsc1Detune(double cents);
    void setOsc1Gain(double gain);
    void setOsc1Pan(double pan);

    // Oscillator 2
    void setOsc2Waveform(Oscillator::Waveform wf);
    void setOsc2Detune(double cents);
    void setOsc2Gain(double gain);
    void setOsc2Pan(double pan);

    // Amplitude envelope
    void setAmpEnvAttack(double sec);
    void setAmpEnvDecay(double sec);
    void setAmpEnvSustain(double level);
    void setAmpEnvRelease(double sec);

    // Filter envelope
    void setFilterEnvAttack(double sec);
    void setFilterEnvDecay(double sec);
    void setFilterEnvSustain(double level);
    void setFilterEnvRelease(double sec);

    // Filter
    void setFilterCutoff(double hertz);
    void setFilterResonance(double res);
    void setFilterType(Filter::FilterType type);

    // Note control
    void noteOn(double note, double velocity);
    void noteOff();

    /** Process one sample, writes to left/right outputs */
    void processSample(double& outLeft, double& outRight);

    bool isPlaying() const { return playing_; }
    bool isReleasing() const { return releasing_; }
    double getNote() const { return note_; }

    /** Get current oscillator outputs for oscilloscope */
    double getOsc1Output() const { return osc1Out_; }
    double getOsc2Output() const { return osc2Out_; }

private:
    double midiToFreq(double note);
    double panToGain(double pan, bool left);

    Oscillator osc1_;
    Oscillator osc2_;
    Envelope ampEnv_;
    Envelope filterEnv_;
    Filter filter_;

    bool playing_ = false;
    bool releasing_ = false;
    double note_ = 60.0;
    double velocity_ = 0.75;

    // For oscilloscope feedback
    double osc1Out_ = 0.0;
    double osc2Out_ = 0.0;
};

} // namespace NeonSynth
