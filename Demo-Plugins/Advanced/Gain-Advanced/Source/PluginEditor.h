/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./GUI/LAF/DialLAF.h"

//==============================================================================
/**
*/
class GainAdvancedAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    GainAdvancedAudioProcessorEditor (GainAdvancedAudioProcessor&);
    ~GainAdvancedAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override
    {
        //popup.showOkCancelBox(juce::AlertWindow::AlertIconType::InfoIcon, "Popup", "BUy My Plugin!", "OK", "Cancel", nullptr, nullptr);
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainAdvancedAudioProcessor& audioProcessor;
    
    /** Dials */
    juce::Slider gainDial;
    void setCommonSliderProps(juce::Slider& slider);
    void setGainDialProps();
    DialStyle customDialLAF;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttach;
    
    /** Shadow */
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    juce::AlertWindow popup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainAdvancedAudioProcessorEditor)
};
