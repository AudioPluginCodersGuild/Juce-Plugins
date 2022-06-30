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
, popup("Popup", "Buy this", juce::AlertWindow::AlertIconType::InfoIcon, &gainDial)
{
    startTimerHz(10);
    
    setCommonSliderProps(gainDial);
    setCommonButtonProps(phaseButton);
    setPhaseButtonProps();
    setGainDialProps();
    setCommonLabelProps(gainLabel);
    setCommonLabelProps(meterLabel);
    setMeterLabelProps();
    setGainLabelProps();
    
    setSize (500, 500);
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(getWidth() * 0.5, getHeight() * 0.5, getWidth() * 1.5, getHeight() * 1.5);
    AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(1.0);
}

GainAdvancedAudioProcessorEditor::~GainAdvancedAudioProcessorEditor()
{
    gainDial.setLookAndFeel(nullptr);
    gainAttach = nullptr;
    stopTimer();
}

//==============================================================================
void GainAdvancedAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(40, 42, 53).darker(0.75f), getHeight(), juce::Colour::fromRGB(40, 42, 53).brighter(0.02f), getHeight() * 0.4));
    g.fillRect(getLocalBounds());
}

void GainAdvancedAudioProcessorEditor::resized()
{
    // Dial
    const auto dialSize = getWidth() * 0.6;
    gainDial.setBounds(getLocalBounds().withSizeKeepingCentre(dialSize, dialSize));
    gainDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, gainDial.getWidth(), 36);
    gainDial.setComponentEffect(&dialShadow);
    
    // Button
    const auto buttonWidth = getWidth() * 0.3;
    const auto buttonHeight = buttonWidth * 0.33;
    phaseButton.setBounds(getLocalBounds().withSizeKeepingCentre(buttonWidth, buttonHeight).withY(setCompOffset(gainDial, false)));
    
    // Label
    const auto labelFontSize = getWidth() * 0.05;
    gainLabel.setFont(juce::Font("Helvetica", labelFontSize, juce::Font::FontStyleFlags::bold));
    
    const auto labelSize = getWidth() * 0.1;
    meterLabel.setBounds(setCompOffset(gainDial, true), gainDial.getY(), labelSize, labelSize);
}

float GainAdvancedAudioProcessorEditor::setCompOffset(juce::Component &comp, bool isX)
{
    if (isX)
    {
        return comp.getX() + comp.getWidth();
    }
    
    else
    {
        return comp.getY() + comp.getHeight();
    }
    
}
