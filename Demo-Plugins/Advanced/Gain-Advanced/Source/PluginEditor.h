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
        gainDial.setColour(juce::Slider::ColourIds::thumbColourId, audioProcessor.getRMS() >= 0.0 ? juce::Colours::indianred : juce::Colour::fromRGB(168, 132, 222));
        
        shadowProperties.colour = audioProcessor.getRMS() >= 0.0 ? juce::Colours::indianred : juce::Colours::black;
        dialShadow.setShadowProperties (shadowProperties);
        gainDial.setComponentEffect(&dialShadow);
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
    
    /** Labels */
    juce::Label gainLabel {"Gain", "Gain"};
    void setCommonLabelProps(juce::Label& label);
    void setGainLabelProps();
    juce::Label meterLabel;
    void setMeterLabelProps();
    
    /** Buttons */
    juce::TextButton phaseButton;
    void setCommonButtonProps(juce::TextButton& button);
    void setPhaseButtonProps();
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> phaseAttach;
    
    /** Shadow */
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    juce::AlertWindow popup;
    juce::MessageBoxOptions messageBoxOptions;
    
    juce::Colour m_textAccentColor = juce::Colours::whitesmoke;
    juce::Colour m_mainCompFillColor = juce::Colour::fromRGB(168, 132, 222).withAlpha(0.6f);
    
    float setCompOffset(juce::Component& comp, bool isX);
    float currentSignal = -60.0;
    
    juce::GlowEffect glow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainAdvancedAudioProcessorEditor)
};
