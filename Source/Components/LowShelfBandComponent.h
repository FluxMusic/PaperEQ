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
    
private:
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    PaperBypass bypassButton {"Bypass"};
    PaperKnob gainSlider, freqSlider, qSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainSliderAttachment, freqSliderAttachment, qSliderAttachment;
};
