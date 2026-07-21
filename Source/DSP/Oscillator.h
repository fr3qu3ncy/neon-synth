/*
  NeonSynth - Polyphonic Subtractive Synthesizer
  Copyright (c) 2026 NeonLabs
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <array>

namespace NeonSynth {

/**
 * Wavetable oscillator with band-limited synthesis.
 * Uses pre-rendered wavetables with linear interpolation.
 * Square and saw waves use BLIT/BLIT2 for alias-free output.
 */
class Oscillator
{
public:
    enum Waveform { Sine, Saw, Square, Triangle };

    Oscillator();
    ~Oscillator() = default;

    void setSampleRate(double rate);
    void setFrequency(double hertz);
    void setDetune(double cents);
    void setWaveform(Waveform wf);
    void setGain(double gain);

    /** Process one audio sample, returns amplitude [-1, 1] */
    double processSample();

private:
    void buildWavetables();
    void buildBLIT(int tableSize, double fundamentalPeriod);
    void buildBLIT2(int tableSize, double fundamentalPeriod);

    double sampleRate_ = 48000.0;
    double frequency_ = 440.0;
    double detune_ = 0.0;
    Waveform waveform_ = Waveform::Saw;
    double gain_ = 0.5;

    /** Phase accumulator [0, 1) */
    double phase_ = 0.0;

    /** Wavetables per waveform type */
    std::vector<double> sineTable_;
    std::vector<double> sawTable_;
    std::vector<double> squareTable_;
    std::vector<double> triangleTable_;

    static constexpr int tableSize_ = 4096;
    static constexpr int blitHarmonics_ = 64;
};

} // namespace NeonSynth
