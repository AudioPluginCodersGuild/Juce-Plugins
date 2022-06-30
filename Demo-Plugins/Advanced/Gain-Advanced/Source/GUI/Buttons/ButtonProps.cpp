/*
  ==============================================================================

    ButtonProps.cpp
    Created: 28 Jun 2022 6:08:16pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../../PluginEditor.h"

void GainAdvancedAudioProcessorEditor::setCommonButtonProps(juce::TextButton &button)
{
    addAndMakeVisible(button);
    button.setColour(juce::TextButton::ColourIds::textColourOnId, m_textAccentColor.brighter(0.25f));
    button.setColour(juce::TextButton::ColourIds::textColourOffId, m_textAccentColor.darker(0.25f));
    button.setColour(juce::TextButton::ColourIds::buttonColourId, m_mainCompFillColor);
    button.setColour(juce::TextButton::ColourIds::buttonOnColourId, m_mainCompFillColor.brighter(0.2f));
    button.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::transparentBlack);
    button.setClickingTogglesState(true);
}

void GainAdvancedAudioProcessorEditor::setPhaseButtonProps()
{
    phaseButton.setButtonText("Phase");
    phaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, phaseID, phaseButton);
}
