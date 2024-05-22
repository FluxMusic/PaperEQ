/*
  ==============================================================================

    HighShelfBandComponent.h
    Created: 21 May 2024 2:07:30pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "PaperKnob.h"

class HighShelfBandComponent : public juce::Component
{
public:
    HighShelfBandComponent(PaperEQAudioProcessor&);
    
private:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    juce::TextButton bypassButton {"Bypass"};
    PaperKnob gainSlider, freqSlider, qSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainSliderAttachment, freqSliderAttachment, qSliderAttachment;
};
