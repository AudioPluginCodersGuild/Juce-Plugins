/*
  ==============================================================================

    LVCompressor.h
    Created: 12 Jul 2022 5:50:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LVCompressor
{
public:
    
    LVCompressor();
    
    void prepare(juce::dsp::ProcessSpec& spec) noexcept;
    
    void process(juce::AudioBuffer<float>& buffer)
    {
        auto data = buffer.getArrayOfWritePointers();
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            {
                {
                    data[ch][sample] = processSample(data[ch][sample]);
                }
            }
        }
    }
    
    float processSample(float input)
    {
        /* Initialise separate attack/release times*/
        auto alphaAttack = std::exp((std::log(9) * -1.0) / (_samplerate * _attack));
        auto alphaRelease = std::exp((std::log(9) * -1.0) / (_samplerate * _release));
        
        const auto x = input;

        auto x_Uni = abs(x);
        auto x_dB = juce::Decibels::gainToDecibels(x_Uni);

        if (x_dB < -96.0)
        {
            x_dB = -96.0;
        }
        
        if (x_dB > _thresh )
        {
            gainSC = _thresh  + (x_dB - _thresh ) / _ratio ;
        }

        else
        {
            gainSC = x_dB;
        }

        gainChange_dB = gainSC - x_dB;

        if (gainChange_dB < gainSmoothPrevious)
        {
            gainSmooth = ((1 - alphaAttack) * gainChange_dB) + (alphaAttack * gainSmoothPrevious);
            currentSignal = gainSmooth;
        }

        else
        {
            gainSmooth = ((1 - alphaRelease) * gainChange_dB) + (alphaRelease * gainSmoothPrevious);
            currentSignal = gainSmooth;
        }
        
        gainSmoothPrevious = gainSmooth;
        
        auto mix = (1.0 - _mix.getNextValue()) * x + (x * juce::Decibels::decibelsToGain(gainSmooth)) * _mix.getNextValue();
        
        return mix;
    }
    
    void setThreshold(float newThresh);
    void setRatio(float newRatio);
    void setAttack(float newAttack);
    void setRelease(float newRelease);
    void setMix(float newMix);
    
private:
    
    float _thresh;
    float _ratio;
    float _attack;
    float _release;
    
    juce::SmoothedValue<float> _mix;
    
    float _samplerate = 44100.f;
    float gainSC = 0.0f;
    float gainSmooth = 0.0f;
    float gainSmoothPrevious = 0.0f;
    float currentSignal = 0.0f;
    float gainChange_dB = 0.0f;
};
