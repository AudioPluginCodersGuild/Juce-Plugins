/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAdvancedAudioProcessorEditor::CompressorAdvancedAudioProcessorEditor (CompressorAdvancedAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
, inputDialLabel("Input", "Input")
, threshDialLabel("Thresh", "Thresh")
, ratioDialLabel("Ratio", "Ratio")
, attackDialLabel("Attack", "Attack")
, releaseDialLabel("Release", "Release")
, limiterThreshDialLabel("Thresh", "Thresh")
, limiterReleaseDialLabel("Release", "Release")
, outputDialLabel("Output", "Output")
{
    for (int i = 0; i < dials.size(); i++)
    {
        setCommonSliderProps(*dials[i]);
    }
    
    attachSliders();
    
    for (int i = 0; i < dialLabels.size(); i++)
    {
        setCommonLabelProps(*dialLabels[i]);
        dialLabels[i]->attachToComponent(dials[i], false);
    }
    
    for (int i = 0; i < groups.size(); i++)
    {
        setGroupProps(*groups[i]);
    }
    
    setSize (1000, 500);
    juce::AudioProcessorEditor::setResizable(true, true);
    juce::AudioProcessorEditor::setResizeLimits(getWidth() * 0.75, getHeight() * 0.75, getWidth() * 1.25, getHeight() * 1.25);
    juce::AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);
}

CompressorAdvancedAudioProcessorEditor::~CompressorAdvancedAudioProcessorEditor()
{
    for (auto& dial : dials)
    {
        dial->setLookAndFeel(nullptr);
    }
    
    dials.clear();
    dials.shrink_to_fit();
}

//==============================================================================
void CompressorAdvancedAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(40, 42, 53).darker(0.75f), getHeight(), juce::Colour::fromRGB(40, 42, 53).brighter(0.02f), getHeight() * 0.4));
    g.fillRect(getLocalBounds());
    
    for (int i = 0; i < dials.size(); i++)
    {
        setCommonSliderProps(*dials[i]);
    }
}

void CompressorAdvancedAudioProcessorEditor::resized()
{
    auto dialSize = getWidth() * 0.18;
    auto mainLeftMargin = getWidth() * 0.23;
    auto leftMargin = getWidth() * 0.03;
    auto secondRowHeight = 1.3;
    auto groupY = 0.1;
    auto groupHeight = 2.4;
    
    inputDial.setBounds(leftMargin, 50, dialSize, dialSize);
    threshDial.setBounds(mainLeftMargin, 50, dialSize, dialSize);
    ratioDial.setBounds(threshDial.getX() + threshDial.getWidth(), 50, dialSize, dialSize);
    
    ioGroup.setBounds(inputDial.getX(), inputDial.getY() * groupY, inputDial.getWidth(),
                      inputDial.getY() + inputDial.getHeight() * groupHeight);
    
    outputDial.setBounds(leftMargin, inputDial.getY() + inputDial.getHeight() * secondRowHeight, dialSize, dialSize);
    attackDial.setBounds(threshDial.getX(), outputDial.getY(), dialSize, dialSize);
    releaseDial.setBounds(ratioDial.getX(), outputDial.getY(), dialSize, dialSize);
    compMixDial.setBounds(ratioDial.getX() + ratioDial.getWidth(), ratioDial.getY(), dialSize, dialSize);
    
    compressorGroup.setBounds(getWidth() * 0.23,
                              threshDial.getY() * groupY, threshDial.getWidth() * 3.0,
                              threshDial.getY() + threshDial.getHeight() * groupHeight);
    
    limiterThreshDial.setBounds(compMixDial.getX() + compMixDial.getWidth() * 1.15,
                                inputDial.getY(), dialSize, dialSize);
    
    limiterReleaseDial.setBounds(limiterThreshDial.getX(),
                                 limiterThreshDial.getY() + limiterThreshDial.getHeight() * secondRowHeight,
                                 dialSize, dialSize);
    
    limiterGroup.setBounds(limiterThreshDial.getX(), limiterThreshDial.getY() * groupY, limiterThreshDial.getWidth(),
                           limiterThreshDial.getY() + limiterThreshDial.getHeight() * groupHeight);
}


