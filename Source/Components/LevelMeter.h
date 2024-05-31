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
    
    float getLevel() { return level; }
private:
    float level { -100.f };
};

class LevelMeter : public juce::Component
{
public:
    LevelMeter();
    
    void paint(juce::Graphics& g) override;
    
    void resized() override;
    
    void setLevels(float leftChannelValue, float rightChannelValue);
    
    float getPeakLevel();
private:
    SingleMeter meterL, meterR;
    
    float oldLevel { -100.f };
};
