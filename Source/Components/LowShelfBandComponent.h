/*
  ==============================================================================

    LowShelfComponent.h
    Created: 21 May 2024 2:02:54pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "PaperKnob.h"
#include "PaperBypass.h"

class LowShelfBandComponent : public juce::Component
{
public:
    LowShelfBandComponent(PaperEQAudioProcessor&);
    
    PaperKnob& getFreqSlider() { return freqSlider; }
    PaperKnob& getGainSlider() { return gainSlider; }
    PaperKnob& getQSlider() { return qSlider; }
    
private:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    PaperKnob gainSlider, freqSlider, qSlider;
    
    PaperBypass bypassButton {"Bypass"};
    juce::AudioProcessorValueTreeState::SliderAttachment gainSliderAttachment, freqSliderAttachment, qSliderAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassButtonAttachment;
};
