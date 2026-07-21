/*
  NeonSynth - State-Variable Filter
  LP/HP/BP with resonance, 12dB/oct rolloff
*/

#pragma once

#include <JuceHeader.h>

namespace NeonSynth {

class Filter
{
public:
    enum FilterType { LowPass, HighPass, BandPass };

    Filter();
    ~Filter() = default;

    void setSampleRate(double rate);
    void setCutoff(double hertz);
    void setResonance(double res);       // 0.0 - 0.9
    void setType(FilterType type);

    /** Process one sample */
    double processSample(double input);

    /** Get magnitude response at a frequency (for UI visualization) */
    double getMagnitudeResponse(double frequency) const;

private:
    double sampleRate_ = 48000.0;
    double cutoff_ = 1000.0;
    double resonance_ = 0.0;
    FilterType type_ = FilterType::LowPass;

    // SVF state variables
    double lp_ = 0.0;
    double hp_ = 0.0;
    double bp_ = 0.0;

    // DC blocker state
    double dcBlocker_ = 0.0;
    double dcPrevInput_ = 0.0;
};

} // namespace NeonSynth
