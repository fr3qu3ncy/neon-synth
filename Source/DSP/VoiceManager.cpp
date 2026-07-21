/*
  NeonSynth - VoiceManager Implementation
*/

#include "VoiceManager.h"

namespace NeonSynth {

VoiceManager::VoiceManager()
{
    for (int i = 0; i < maxVoices_; ++i)
    {
        voiceFree_[i] = true;
        voiceNote_[i] = -1.0;
    }
}

void VoiceManager::setSampleRate(double rate)
{
    for (auto& voice : voices_)
        voice.setSampleRate(rate);
}

void VoiceManager::noteOn(double note, double velocity)
{
    // First, check if a voice is already playing this exact note - retrigger it
    for (int i = 0; i < maxVoices_; ++i)
    {
        if (!voiceFree_[i] && std::abs(voiceNote_[i] - note) < 0.5)
        {
            voices_[i].noteOn(note, velocity);
            return;
        }
    }

    // Find a free voice
    for (int i = 0; i < maxVoices_; ++i)
    {
        if (voiceFree_[i])
        {
            voices_[i].noteOn(note, velocity);
            voiceFree_[i] = false;
            voiceNote_[i] = note;
            activeVoices_++;
            return;
        }
    }

    // No free voices - steal the oldest non-playing voice
    stealVoice();
}

void VoiceManager::stealVoice()
{
    // Find oldest voice that's not currently playing (in release or off)
    int stealIndex = -1;
    double oldestTime = -1.0;

    for (int i = 0; i < maxVoices_; ++i)
    {
        if (!voiceFree_[i] && !voices_[i].isPlaying())
        {
            if (voices_[i].isReleasing() || stealIndex == -1)
            {
                stealIndex = i;
                break; // Take first available releasing voice
            }
        }
    }

    if (stealIndex == -1)
    {
        // Emergency: steal voice 0
        stealIndex = 0;
    }

    voices_[stealIndex].noteOn(voiceNote_[stealIndex], 0.75);
    voiceNote_[stealIndex] = voiceNote_[stealIndex];
}

void VoiceManager::noteOff(double note)
{
    for (int i = 0; i < maxVoices_; ++i)
    {
        if (!voiceFree_[i] && std::abs(voiceNote_[i] - note) < 0.5)
        {
            voices_[i].noteOff();
            return;
        }
    }
}

void VoiceManager::processSample(double& outLeft, double& outRight)
{
    outLeft = 0.0;
    outRight = 0.0;

    int voicesActive = 0;

    for (int i = 0; i < maxVoices_; ++i)
    {
        if (voiceFree_[i])
            continue;

        double vLeft = 0.0, vRight = 0.0;
        voices_[i].processSample(vLeft, vRight);

        outLeft += vLeft;
        outRight += vRight;

        // Check if voice is done
        if (!voices_[i].isPlaying() && !voices_[i].isReleasing())
        {
            voiceFree_[i] = true;
            voiceNote_[i] = -1.0;
            if (activeVoices_ > 0) activeVoices_--;
        }
        else
        {
            voicesActive++;
        }
    }

    // Normalize by number of active voices to prevent clipping
    if (voicesActive > 1)
    {
        double norm = 1.0 / std::sqrt(static_cast<double>(voicesActive));
        outLeft *= norm;
        outRight *= norm;
    }

    // Hard clip as safety
    outLeft = juce::jlimit(-1.0, 1.0, outLeft);
    outRight = juce::jlimit(-1.0, 1.0, outRight);
}

Voice* VoiceManager::getVoice(int index)
{
    if (index >= 0 && index < maxVoices_)
        return &voices_[index];
    return nullptr;
}

int VoiceManager::getActiveVoiceCount() const
{
    return activeVoices_;
}

} // namespace NeonSynth
