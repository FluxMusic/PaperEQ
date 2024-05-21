/*
  ==============================================================================

    OutputComponent.h
    Created: 21 May 2024 7:16:48pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "LevelMeter.h"

class OutputComponent : public juce::Component, public juce::Timer
{
public:
    OutputComponent(PaperEQAudioProcessor& p);
    
    void paint(juce::Graphics& g) override;
    
    void resized() override;
    
    void timerCallback() override;
    
private:
    PaperEQAudioProcessor& audioProcessor;
    
    LevelMeter inputMeter, outputMeter;
    
    juce::Slider outputGain {juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::AudioProcessorValueTreeState::SliderAttachment outputGainAttachment;
};
