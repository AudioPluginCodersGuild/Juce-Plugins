/*
  ==============================================================================

    GroupProps.cpp
    Created: 7 Jul 2022 7:18:42pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../../PluginEditor.h"

void CompressorAdvancedAudioProcessorEditor::setGroupProps(juce::GroupComponent &group)
{
    addAndMakeVisible(group);
    ioGroup.setText("IO");
    compressorGroup.setText("Compressor");
    limiterGroup.setText("Limiter");
}
