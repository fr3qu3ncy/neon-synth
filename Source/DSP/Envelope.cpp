/*
  NeonSynth - ADSR Envelope Implementation
*/

#include "Envelope.h"

namespace NeonSynth {

Envelope::Envelope() = default;

void Envelope::setSampleRate(double rate)
{
    sampleRate_ = rate;
}

void Envelope::setAttack(double seconds)
{
    attack_ = juce::jlimit(0.001, 5.0, seconds);
}

void Envelope::setDecay(double seconds)
{
    decay_ = juce::jlimit(0.001, 5.0, seconds);
}

void Envelope::setSustain(double level)
{
    sustain_ = juce::jlimit(0.0, 1.0, level);
}

void Envelope::setRelease(double seconds)
{
    release_ = juce::jlimit(0.001, 10.0, seconds);
}

double Envelope::timeConstant(double seconds)
{
    // One-pole exponential: coeff = exp(-1 / (seconds * sr))
    return std::exp(-1.0 / (seconds * sampleRate_));
}

void Envelope::noteOn()
{
    phase_ = Phase::Attack;
    value_ = 0.0;
}

void Envelope::noteOff()
{
    if (phase_ != Phase::Off)
        phase_ = Phase::Release;
}

double Envelope::processSample()
{
    switch (phase_)
    {
        case Phase::Attack:
        {
            double coeff = timeConstant(attack_);
            value_ += (1.0 - value_) * (1.0 - coeff);
            if (value_ >= 0.999)
            {
                value_ = 1.0;
                phase_ = Phase::Decay;
            }
            break;
        }

        case Phase::Decay:
        {
            double coeff = timeConstant(decay_);
            value_ += (sustain_ - value_) * (1.0 - coeff);
            if (std::abs(value_ - sustain_) < 0.001)
            {
                value_ = sustain_;
                phase_ = Phase::Sustain;
            }
            break;
        }

        case Phase::Sustain:
            value_ = sustain_;
            break;

        case Phase::Release:
        {
            double coeff = timeConstant(release_);
            value_ *= coeff;
            if (value_ < 0.0001)
            {
                value_ = 0.0;
                phase_ = Phase::Off;
            }
            break;
        }

        case Phase::Off:
            value_ = 0.0;
            break;
    }

    return value_;
}

} // namespace NeonSynth
