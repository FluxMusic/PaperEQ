/*
  ==============================================================================

    ResponseCurveDisplay.h
    Created: 30 May 2024 2:16:35pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "ResponseCurveComponent.h"

class ResponseCurveDisplay : public juce::Component
{
public:
    ResponseCurveDisplay(PaperEQAudioProcessor& p);
private:
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    ResponseCurveComponent responseCurveComponent;
};
