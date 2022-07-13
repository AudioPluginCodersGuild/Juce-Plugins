/*
  ==============================================================================

    SliderProps.cpp
    Created: 5 Jul 2022 6:40:31pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../../PluginEditor.h"

void CompressorAdvancedAudioProcessorEditor::setCommonSliderProps(juce::Slider &slider)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 36);
    slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::whitesmoke.darker(0.8));
    slider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.brighter(0.25));
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 1.0f).darker(1.0f));
    slider.setLookAndFeel(&customDialLAF);
    
    shadowProperties.radius = 24;
    shadowProperties.offset = juce::Point<int> (0, 0);
    shadowProperties.colour = juce::Colours::black;
    dialShadow.setShadowProperties (shadowProperties);
    slider.setComponentEffect(&dialShadow);
}

void CompressorAdvancedAudioProcessorEditor::attachSliders()
{
    inputAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputID, inputDial);
    
    threshAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, threshID, threshDial);
    
    ratioAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, ratioID, ratioDial);
    
    attackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, attackID, attackDial);
    
    releaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, releaseID, releaseDial);
    
    limiterThreshAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, LThreshID, limiterThreshDial);
    
    limiterReleaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, LReleaseID, limiterReleaseDial);
    
    outputAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, outputID, outputDial);
    
    compMixAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, compMixID, compMixDial);
}
