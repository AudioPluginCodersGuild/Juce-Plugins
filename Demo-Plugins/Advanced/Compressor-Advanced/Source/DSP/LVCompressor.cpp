/*
  ==============================================================================

    LVCompressor.cpp
    Created: 12 Jul 2022 5:50:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "LVCompressor.h"

LVCompressor::LVCompressor()
{
    
}


void LVCompressor::prepare(juce::dsp::ProcessSpec &spec) noexcept
{
    //_thresh.reset(spec.sampleRate, 0.02);
    //_ratio.reset(spec.sampleRate, 0.02);
    //_attack.reset(spec.sampleRate, 0.02);
    //_release.reset(spec.sampleRate, 0.02);
    _mix.reset(spec.sampleRate, 0.02);
    _samplerate = spec.sampleRate;
}

void LVCompressor::setThreshold(float newThresh)
{
    //_thresh.setTargetValue(newThresh);
    _thresh = newThresh;
}

void LVCompressor::setRatio(float newRatio)
{
    //_ratio.setTargetValue(newRatio);
    _ratio = newRatio;
}

void LVCompressor::setAttack(float newAttack)
{
    //_attack.setTargetValue(newAttack / 1000.0f);
    _attack = newAttack / 1000.0f;
}

void LVCompressor::setRelease(float newRelease)
{
    //_release.setTargetValue(newRelease / 1000.0f);
    _release = newRelease / 1000.0f;
}

void LVCompressor::setMix(float newMix)
{
    _mix.setTargetValue(newMix);
}
