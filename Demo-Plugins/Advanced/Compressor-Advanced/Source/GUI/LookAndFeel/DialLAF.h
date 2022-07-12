/*
  ==============================================================================

    DialLAF.h
    Created: 23 Jun 2022 6:33:24pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DialStyle : public juce::LookAndFeel_V4
{
public:
    DialStyle();
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawLabel (juce::Graphics& g, juce::Label& label) override;
    
private:
    float sliderWidth;
};
