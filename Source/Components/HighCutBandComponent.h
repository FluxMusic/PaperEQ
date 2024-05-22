/*
  ==============================================================================

    HighCutBandComponent.h
    Created: 21 May 2024 2:11:59pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "PaperKnob.h"

class HighCutBandComponent : public juce::Component
{
public:
    HighCutBandComponent(PaperEQAudioProcessor&);
    
private:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    juce::TextButton bypassButton {"Bypass"};
    PaperKnob freqSlider, slopeSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment freqSliderAttachment, slopeSliderAttachment;
};
