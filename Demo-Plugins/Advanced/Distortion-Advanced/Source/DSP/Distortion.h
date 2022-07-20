/*
  ==============================================================================

    Distortion.h
    Created: 19 Jul 2022 6:12:44pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <typename SampleType>
class Distortion
{
public:
    
    Distortion();
    
    void prepare(juce::dsp::ProcessSpec& spec);
    
    void reset();
    
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock      = context.getOutputBlock();
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples  = outputBlock.getNumSamples();

        jassert (inputBlock.getNumChannels() == numChannels);
        jassert (inputBlock.getNumSamples()  == numSamples);
        
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples  = inputBlock .getChannelPointer (channel);
            auto* outputSamples = outputBlock.getChannelPointer (channel);

            for (size_t i = 0; i < numSamples; ++i)
            {
                outputSamples[i] = processSample (inputSamples[i]);
            }
        }
    }
    
    SampleType processSample (SampleType inputSample) noexcept
    {
        switch (_model)
        {
            case DistortionModel::kHard:
            {
                return processHardClipper(inputSample);
                break;
            }

            case DistortionModel::kSoft:
            {
                return processSoftClipper(inputSample);
                break;
            }

            case DistortionModel::kSaturation:
            {
                return processSaturation(inputSample);
                break;
            }
        }
    }
    
    SampleType processHardClipper(SampleType inputSample)
    {
        auto wetSignal = inputSample * juce::Decibels::decibelsToGain(_input.getNextValue());
        
        if (std::abs(wetSignal) > 0.99)
        {
            wetSignal *= 0.99 / std::abs(wetSignal);
        }
        
        auto mix = (1.0 - _mix.getNextValue()) * inputSample + wetSignal * _mix.getNextValue();
        
        return mix * juce::Decibels::decibelsToGain(_output.getNextValue());
    }
    
    SampleType processSoftClipper(SampleType inputSample)
    {
        return inputSample;
    }
    
    SampleType processSaturation(SampleType inputSample)
    {
        return inputSample;
    }
    
    enum class DistortionModel
    {
        kHard,
        kSoft,
        kSaturation
    };
    
    void setDrive(SampleType newDrive);
    void setMix(SampleType newMix);
    void setOutput(SampleType newOutput);
    
    void setDistortionModel(DistortionModel newModel);
    
private:
    juce::SmoothedValue<float> _input;
    juce::SmoothedValue<float> _mix;
    juce::SmoothedValue<float> _output;
    
    juce::dsp::LinkwitzRileyFilter<float> _dcFilter;
    
    float _sampleRate = 44100.0f;
    
    DistortionModel _model = DistortionModel::kHard;
};
