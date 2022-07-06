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
, outputDialLabel("Output", "Output")
{
    for (int i = 0; i < dials.size(); i++)
    {
        setCommonSliderProps(*dials[i]);
    }
    
    for (int i = 0; i < dialLabels.size(); i++)
    {
        setCommonLabelProps(*dialLabels[i]);
        dialLabels[i]->attachToComponent(dials[i], false);
    }
    
    setSize (1000, 500);
    juce::AudioProcessorEditor::setResizable(true, true);
    juce::AudioProcessorEditor::setResizeLimits(getWidth() * 0.75, getHeight() * 0.75, getWidth() * 1.25, getHeight() * 1.25);
    juce::AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0);
}

CompressorAdvancedAudioProcessorEditor::~CompressorAdvancedAudioProcessorEditor()
{
    dials.clear();
    dials.shrink_to_fit();
}

//==============================================================================
void CompressorAdvancedAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(40, 42, 53).darker(0.75f), getHeight(), juce::Colour::fromRGB(40, 42, 53).brighter(0.02f), getHeight() * 0.4));
    g.fillRect(getLocalBounds());
}

void CompressorAdvancedAudioProcessorEditor::resized()
{
    
    auto dialSize = getWidth() * 0.2;
    auto mainLeftMargin = getWidth() * 0.15;
    auto leftMargin = getWidth() * 0.03;
    
    inputDial.setBounds(leftMargin, 50, dialSize, dialSize);
    threshDial.setBounds(inputDial.getX() + inputDial.getWidth(), 50, dialSize, dialSize);
    ratioDial.setBounds(threshDial.getX() + threshDial.getWidth(), 50, dialSize, dialSize);
    
    outputDial.setBounds(leftMargin, inputDial.getY() + inputDial.getHeight(), dialSize, dialSize);
    attackDial.setBounds(threshDial.getX(), outputDial.getY(), dialSize, dialSize);
    releaseDial.setBounds(ratioDial.getX(), outputDial.getY(), dialSize, dialSize);
    
//    // Row 1
//    juce::FlexBox flexboxRowOne;
//    flexboxRowOne.flexDirection = juce::FlexBox::Direction::row;
//    flexboxRowOne.flexWrap = juce::FlexBox::Wrap::noWrap;
//    flexboxRowOne.alignContent = juce::FlexBox::AlignContent::center;
//
//    juce::Array<juce::FlexItem> itemArrayRowOne;
//
//    itemArrayRowOne.add(juce::FlexItem(dialSize, dialSize, inputDial)
//                        .withMargin(juce::FlexItem::Margin(0, 0, 0, leftMargin)));
//    itemArrayRowOne.add(juce::FlexItem(dialSize, dialSize, threshDial)
//                        .withMargin(juce::FlexItem::Margin(0, 0, 0, mainLeftMargin)));
//    itemArrayRowOne.add(juce::FlexItem(dialSize, dialSize, ratioDial)
//                        .withMargin(juce::FlexItem::Margin(0, 0, 0, 0)));
//
//    flexboxRowOne.items = itemArrayRowOne;
//    flexboxRowOne.performLayout(getLocalBounds().withY(0).withHeight(getHeight() * 0.5));
//
//    // Row 2
//    juce::FlexBox flexboxRowTwo;
//    flexboxRowTwo.flexDirection = juce::FlexBox::Direction::row;
//    flexboxRowTwo.flexWrap = juce::FlexBox::Wrap::noWrap;
//    flexboxRowTwo.alignContent = juce::FlexBox::AlignContent::center;
//
//    juce::Array<juce::FlexItem> itemArrayRowTwo;
//
//    itemArrayRowTwo.add(juce::FlexItem(dialSize, dialSize, outputDial)
//                        .withMargin(juce::FlexItem::Margin(0, 0, 0, leftMargin)));
//    itemArrayRowTwo.add(juce::FlexItem(dialSize, dialSize, attackDial)
//                        .withMargin(juce::FlexItem::Margin(0, 0, 0, mainLeftMargin)));
//    itemArrayRowTwo.add(juce::FlexItem(dialSize, dialSize, releaseDial)
//                        .withMargin(juce::FlexItem::Margin(0, 0, 0, 0)));
//
//    flexboxRowTwo.items = itemArrayRowTwo;
//    flexboxRowTwo.performLayout(getLocalBounds().withY(getHeight() * 0.5).withHeight(getHeight() * 0.5));
    
}


