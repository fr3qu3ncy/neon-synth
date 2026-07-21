/*
  NeonSynth - ADSR Envelope Generator
*/

#pragma once

#include <JuceHeader.h>

namespace NeonSynth {

/**
 * ADSR envelope generator.
 * Phases: Attack -> Decay -> Sustain (hold) -> Release -> Off
 */
class Envelope
{
public:
    enum Phase { Attack, Decay, Sustain, Release, Off };

    Envelope();
    ~Envelope() = default;

    void setSampleRate(double rate);
    void setAttack(double seconds);
    void setDecay(double seconds);
    void setSustain(double level);   // 0.0 - 1.0
    void setRelease(double seconds);

    void noteOn();
    void noteOff();

    /** Process one sample, returns envelope value [0, 1] */
    double processSample();

    Phase getPhase() const { return phase_; }
    double getValue() const { return value_; }

private:
    double timeConstant(double seconds);

    double sampleRate_ = 48000.0;
    double attack_ = 0.01;
    double decay_ = 0.3;
    double sustain_ = 0.7;
    double release_ = 0.5;

    Phase phase_ = Phase::Off;
    double value_ = 0.0;
};

} // namespace NeonSynth
