/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CompressorAdvancedAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CompressorAdvancedAudioProcessorEditor (CompressorAdvancedAudioProcessor&);
    ~CompressorAdvancedAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CompressorAdvancedAudioProcessor& audioProcessor;
    
    juce::Slider inputDial;
    juce::Slider threshDial;
    juce::Slider ratioDial;
    juce::Slider attackDial;
    juce::Slider releaseDial;
    juce::Slider outputDial;
    
    std::vector<juce::Slider*> dials =
    {
        &inputDial, &threshDial, &ratioDial,
        &attackDial, &releaseDial, &outputDial,
    };
    
    juce::Label inputDialLabel;
    juce::Label threshDialLabel;
    juce::Label ratioDialLabel;
    juce::Label attackDialLabel;
    juce::Label releaseDialLabel;
    juce::Label outputDialLabel;
    
    std::vector<juce::Label*> dialLabels =
    {
        &inputDialLabel, &threshDialLabel, &ratioDialLabel,
        &attackDialLabel, &releaseDialLabel, &outputDialLabel,
    };
    
    void setCommonSliderProps(juce::Slider& slider);
    void setCommonLabelProps(juce::Label& label);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAdvancedAudioProcessorEditor)
};