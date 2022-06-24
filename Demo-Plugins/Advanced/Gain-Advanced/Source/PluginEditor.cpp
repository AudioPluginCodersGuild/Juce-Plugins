/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainAdvancedAudioProcessorEditor::GainAdvancedAudioProcessorEditor (GainAdvancedAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
, popup("Popup", "Buy this", juce::AlertWindow::AlertIconType::InfoIcon)
{
    //startTimer(5000);
    setCommonSliderProps(gainDial);
    setGainDialProps();
    setSize (800, 600);
}

GainAdvancedAudioProcessorEditor::~GainAdvancedAudioProcessorEditor()
{
    gainDial.setLookAndFeel(nullptr);
    gainAttach = nullptr;
}

//==============================================================================
void GainAdvancedAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour::fromRGB(40, 42, 53));
}

void GainAdvancedAudioProcessorEditor::resized()
{
    const auto dialSize = getWidth() * 0.3;
    gainDial.setBounds(getLocalBounds().withSizeKeepingCentre(dialSize, dialSize));
    gainDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, gainDial.getWidth(), 36);
    gainDial.setComponentEffect(&dialShadow);
}
