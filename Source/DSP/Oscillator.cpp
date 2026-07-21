/*
  NeonSynth - Oscillator Implementation
  Wavetable synthesis with band-limited BLIT/BLIT2 waveforms
*/

#include "Oscillator.h"

namespace NeonSynth {

Oscillator::Oscillator()
{
    buildWavetables();
}

void Oscillator::setSampleRate(double rate)
{
    sampleRate_ = rate;
}

void Oscillator::setFrequency(double hertz)
{
    frequency_ = juce::jlimit(20.0, 20000.0, hertz);
}

void Oscillator::setDetune(double cents)
{
    detune_ = juce::jlimit(-50.0, 50.0, cents);
}

void Oscillator::setWaveform(Waveform wf)
{
    waveform_ = wf;
}

void Oscillator::setGain(double gain)
{
    gain_ = juce::jlimit(0.0, 1.0, gain);
}

void Oscillator::buildWavetables()
{
    const int N = tableSize_;
    sineTable_.resize(N);
    sawTable_.resize(N);
    squareTable_.resize(N);
    triangleTable_.resize(N);

    // Sine - pure
    for (int i = 0; i < N; ++i)
    {
        double angle = 2.0 * juce::MathConstants<double>::pi * i / N;
        sineTable_[i] = std::sin(angle);
    }

    // Saw - BLIT (Band-limited Impulse Train) integration
    buildBLIT(N, 1.0);

    // Square - BLIT2
    buildBLIT2(N, 1.0);

    // Triangle - integrate sine (or integrate saw with sign flip)
    for (int i = 0; i < N; ++i)
    {
        double t = static_cast<double>(i) / N;
        double sum = 0.0;
        for (int h = 1; h <= blitHarmonics_; h += 2)
        {
            double h2 = h * h;
            double sign = ((h % 4) == 1) ? 1.0 : -1.0;
            sum += sign * std::sin(2.0 * juce::MathConstants<double>::pi * h * t) / h2;
        }
        triangleTable_[i] = sum * (4.0 / juce::MathConstants<double>::pi);
    }
}

void Oscillator::buildBLIT(int tableSize, double fundamentalPeriod)
{
    // BLIT: sum of sinusoidal impulses, then integrate for saw
    std::vector<double> blit(tableSize, 0.0);

    for (int h = 1; h <= blitHarmonics_; ++h)
    {
        double impulse = -2.0 * juce::MathConstants<double>::pi * h
            * std::sin(2.0 * juce::MathConstants<double>::pi * h * fundamentalPeriod)
            / tableSize;
        for (int i = 0; i < tableSize; ++i)
        {
            blit[i] += std::sin(2.0 * juce::MathConstants<double>::pi * h * i / tableSize) * impulse;
        }
    }

    // Integrate BLIT to get saw
    double integrator = 0.0;
    for (int i = 0; i < tableSize; ++i)
    {
        integrator += blit[i];
        sawTable_[i] = integrator;
    }
}

void Oscillator::buildBLIT2(int tableSize, double fundamentalPeriod)
{
    // BLIT2: two offset BLITs, then integrate for square
    std::vector<double> blit1(tableSize, 0.0);
    std::vector<double> blit2(tableSize, 0.0);

    for (int h = 1; h <= blitHarmonics_; ++h)
    {
        double impulse1 = -2.0 * juce::MathConstants<double>::pi * h
            * std::sin(2.0 * juce::MathConstants<double>::pi * h * fundamentalPeriod)
            / tableSize;
        double impulse2 = -2.0 * juce::MathConstants<double>::pi * h
            * std::sin(2.0 * juce::MathConstants<double>::pi * h * (fundamentalPeriod + 0.5))
            / tableSize;

        for (int i = 0; i < tableSize; ++i)
        {
            double angle = 2.0 * juce::MathConstants<double>::pi * h * i / tableSize;
            blit1[i] += std::sin(angle) * impulse1;
            blit2[i] += std::sin(angle) * impulse2;
        }
    }

    double int1 = 0.0, int2 = 0.0;
    for (int i = 0; i < tableSize; ++i)
    {
        int1 += blit1[i];
        int2 += blit2[i];
        squareTable_[i] = int1 - int2;
    }
}

double Oscillator::processSample()
{
    // Apply detune
    double effFreq = frequency_ * std::pow(2.0, detune_ / 1200.0);

    // Phase increment
    double phaseInc = effFreq / sampleRate_;
    phase_ += phaseInc;
    while (phase_ >= 1.0) phase_ -= 1.0;
    while (phase_ < 0.0) phase_ += 1.0;

    // Select wavetable
    const std::vector<double>* table = nullptr;
    switch (waveform_)
    {
        case Waveform::Sine:     table = &sineTable_; break;
        case Waveform::Saw:      table = &sawTable_; break;
        case Waveform::Square:   table = &squareTable_; break;
        case Waveform::Triangle: table = &triangleTable_; break;
    }

    if (!table || table->empty())
        return 0.0;

    // Interpolated table read
    double idx = phase_ * static_cast<double>(table->size());
    int i0 = static_cast<int>(std::floor(idx));
    int i1 = (i0 + 1) % table->size();
    double frac = idx - std::floor(idx);

    double sample = table->at(i0) * (1.0 - frac) + table->at(i1) * frac;

    return sample * gain_;
}

} // namespace NeonSynth
