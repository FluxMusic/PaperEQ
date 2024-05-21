/*
  ==============================================================================

    PeakBandComponent.h
    Created: 21 May 2024 1:17:21pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PeakBandComponent : public juce::Component
{
public:
    PeakBandComponent();
    
private:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    juce::TextButton bypassButton {"Bypass"};
    juce::Slider gainSlider, freqSlider, qSlider;
};
