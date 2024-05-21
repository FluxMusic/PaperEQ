/*
  ==============================================================================

    ResponseCurveComponent.h
    Created: 21 May 2024 12:32:37pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ResponseCurveComponent : public juce::Component
{
public:
    ResponseCurveComponent();
private:
    void paint(juce::Graphics& g) override;
};
