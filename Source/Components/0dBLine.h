/*
  ==============================================================================

    0dBLine.h
    Created: 9 Apr 2024 3:37:26pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ZerodBLine : public juce::Component
{
public:
    ZerodBLine()
    {
        setInterceptsMouseClicks(false, false);
    }
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds();
        
        g.setColour(juce::Colours::grey);
        g.drawHorizontalLine(bounds.getHeight() / 2, bounds.getX(), bounds.getWidth());
    }
};
