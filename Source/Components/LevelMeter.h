/*
  ==============================================================================

    LevelMeter.h
    Created: 21 May 2024 5:51:40pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

class SingleMeter : public juce::Component
{
public:
    SingleMeter();
    
    void paint(juce::Graphics& g) override;
    
    void setLevel(float value) { level = value; }
private:
    float level { -100.f };
};

class LevelMeter : public juce::Component
{
public:
    LevelMeter(PaperEQAudioProcessor& p);
    
    void paint(juce::Graphics& g) override;
    
    void resized() override;
    
    void setLevels(float leftChannelValue, float rightChannelValue);
private:
    PaperEQAudioProcessor& audioProcessor;
    
    SingleMeter meterL, meterR;
};
