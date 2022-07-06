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
}
