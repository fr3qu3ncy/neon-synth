/*
  NeonSynth - State-Variable Filter Implementation
  Based on the standard SVF topology with DC blocking for stability
*/

#include "Filter.h"

namespace NeonSynTh {

Filter::Filter() = default;

void Filter::setSampleRate(double rate)
{
    sampleRate_ = rate;
}

void Filter::setCutoff(double hertz)
{
    cutoff_ = juce::jlimit(20.0, 20000.0, hertz);
}

void Filter::setResonance(double res)
{
    resonance_ = juce::jlimit(0.0, 0.9, res);
}

void Filter::setType(FilterType type)
{
    type_ = type;
}

double Filter::processSample(double input)
{
    // DC blocker on input (prevents runaway)
    dcBlocker_ = input - bp_;
    dcBlocker_ *= 0.999;
    input = dcBlocker_;

    // Compute SVF coefficients
    double fc = cutoff_;
    double omega = 2.0 * MathConstants<double>::pi * fc / sampleRate_;

    // Clamp omega to prevent instability
    omega = juce::jlimit(0.01, 0.99, omega);

    double sinW = std::sin(omega);
    double cosW = std::cos(omega);
    double damp = resonance_;

    // State-variable filter equations
    double lpNew = lp_ + sinW * bp_;
    double hpNew = input - lpNew - damp * bp_;
    double bpNew = bp_ + sinW * hpNew;

    lp_ = lpNew;
    hp_ = hpNew;
    bp_ = bpNew;

    // Output selection
    switch (type_)
    {
        case FilterType::LowPass:  return lp_;
        case FilterType::HighPass: return hp_;
        case FilterType::BandPass: return bp_;
    }
    return lp_;
}

double Filter::getMagnitudeResponse(double frequency) const
{
    // Compute frequency response magnitude (for visualization)
    double fc = cutoff_;
    double f = frequency;
    double ratio = f / fc;

    // 2nd order filter magnitude
    double mag = 1.0 / std::sqrt(
        std::pow(1.0 - ratio * ratio, 2) +
        std::pow(ratio / (1.0 - resonance_), 2)
    );

    // For HP, flip the response
    if (type_ == FilterType::HighPass)
        mag = 1.0 - mag;

    // For BP, peak at cutoff
    if (type_ == FilterType::BandPass)
    {
        double bpMag = (ratio / (1.0 - resonance_)) /
            std::sqrt(std::pow(1.0 - ratio * ratio, 2) +
                     std::pow(ratio / (1.0 - resonance_), 2));
        mag = bpMag;
    }

    return mag;
}

} // namespace NeonSynth
