/*
  ==============================================================================

    LabelProps.cpp
    Created: 28 Jun 2022 6:41:50pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../../PluginEditor.h"

void GainAdvancedAudioProcessorEditor::setCommonLabelProps(juce::Label &label)
{
    addAndMakeVisible(label);
    label.setFont(juce::Font("Helvetica", 16.0f, juce::Font::FontStyleFlags::bold));
    label.setJustificationType(juce::Justification::centred);
}

void GainAdvancedAudioProcessorEditor::setGainLabelProps()
{
    gainLabel.attachToComponent(&gainDial, false);
    
}

void GainAdvancedAudioProcessorEditor::setMeterLabelProps()
{
    glow.setGlowProperties(128, juce::Colours::green.brighter(0.5f));
    meterLabel.setComponentEffect(&glow);
}
