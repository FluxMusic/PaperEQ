/*
  ==============================================================================

    LowCutBandComponent.h
    Created: 21 May 2024 1:54:19pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

class LowCutBandComponent : public juce::Component
{
public:
    LowCutBandComponent(PaperEQAudioProcessor&);
    
private:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    juce::TextButton bypassButton {"Bypass"};
    juce::Slider freqSlider, slopeSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment freqSliderAttachment, slopeSliderAttachment;
};
