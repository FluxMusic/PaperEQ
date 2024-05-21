/*
  ==============================================================================

    ResponseCurveComponent.h
    Created: 21 May 2024 12:32:37pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

class ResponseCurveComponent : public juce::Component
{
public:
    ResponseCurveComponent(PaperEQAudioProcessor& p);
private:
    void paint(juce::Graphics& g) override;
    
    PaperEQAudioProcessor& audioProcessor;
};
