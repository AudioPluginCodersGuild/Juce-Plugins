/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./GUI/LookAndFeel/DialLAF.h"

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
    juce::Slider limiterThreshDial;
    juce::Slider limiterReleaseDial;
    juce::Slider outputDial;
    
    DialStyle customDialLAF;
    
    std::vector<juce::Slider*> dials =
    {
        &inputDial, &threshDial, &ratioDial,
        &attackDial, &releaseDial, &limiterThreshDial,
        &limiterReleaseDial, &outputDial
    };
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> threshAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> limiterThreshAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> limiterReleaseAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttach;
    
    void attachSliders();
    
    juce::Label inputDialLabel;
    juce::Label threshDialLabel;
    juce::Label ratioDialLabel;
    juce::Label attackDialLabel;
    juce::Label releaseDialLabel;
    juce::Label limiterThreshDialLabel;
    juce::Label limiterReleaseDialLabel;
    juce::Label outputDialLabel;
    
    std::vector<juce::Label*> dialLabels =
    {
        &inputDialLabel, &threshDialLabel, &ratioDialLabel,
        &attackDialLabel, &releaseDialLabel, &limiterThreshDialLabel,
        &limiterReleaseDialLabel, &outputDialLabel
    };
    
    juce::GroupComponent ioGroup;
    juce::GroupComponent compressorGroup;
    juce::GroupComponent limiterGroup;
    
    std::vector<juce::GroupComponent*> groups =
    {
        &ioGroup, &compressorGroup, &limiterGroup,
    };
    
    void setCommonSliderProps(juce::Slider& slider);
    void setCommonLabelProps(juce::Label& label);
    void setGroupProps(juce::GroupComponent& group);
    
    /** Shadow */
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAdvancedAudioProcessorEditor)
};
