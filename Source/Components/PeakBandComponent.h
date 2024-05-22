/*
  ==============================================================================

    PeakBandComponent.h
    Created: 21 May 2024 1:17:21pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "PaperKnob.h"

class PeakBandComponent : public juce::Component
{
public:
    PeakBandComponent(PaperEQAudioProcessor&);
    
private:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    juce::TextButton bypassButton {"Bypass"};
    PaperKnob gainSlider, freqSlider, qSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainSliderAttachment, freqSliderAttachment, qSliderAttachment;
};
