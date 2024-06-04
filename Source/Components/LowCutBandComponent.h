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
    
    PaperKnob& getFreqSlider() { return freqSlider; }
    PaperKnob& getSlopeSlider() { return slopeSlider; }
    
private:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    PaperKnob freqSlider, slopeSlider;
    
    PaperBypass bypassButton {"Bypass"};
    juce::AudioProcessorValueTreeState::SliderAttachment freqSliderAttachment, slopeSliderAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassButtonAttachment;
};
