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
#include "PaperKnob.h"
#include "PaperBypass.h"

class LowCutBandComponent : public juce::Component
{
public:
    LowCutBandComponent(PaperEQAudioProcessor&);
    
private:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    PaperBypass bypassButton {"Bypass"};
    PaperKnob freqSlider, slopeSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment freqSliderAttachment, slopeSliderAttachment;
};
