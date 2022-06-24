/*
  ==============================================================================

    SliderProps.cpp
    Created: 23 Jun 2022 6:25:35pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../../PluginEditor.h"

void GainAdvancedAudioProcessorEditor::setCommonSliderProps(juce::Slider &slider)
{
    
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 96, 36);
    slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
    slider.setLookAndFeel(&customDialLAF);
    
    shadowProperties.radius = 24;
    shadowProperties.offset = juce::Point<int> (0, 0);
    shadowProperties.colour = juce::Colours::black;
    dialShadow.setShadowProperties (shadowProperties);
    slider.setComponentEffect(&dialShadow);
}

void GainAdvancedAudioProcessorEditor::setGainDialProps()
{
    gainDial.setRange(-60.0, 24.0, 0.01);
    gainDial.setTextValueSuffix(" dB");
    
    gainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, gainID, gainDial);
}
