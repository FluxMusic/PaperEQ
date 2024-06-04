/*
  ==============================================================================

    ResponseCurve.h
    Created: 9 Apr 2024 2:02:10pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

class ResponseCurve : public juce::Component
{
public:
    ResponseCurve(PaperEQAudioProcessor&);
    
    void paint(juce::Graphics& g) override;
    
private:
    PaperEQAudioProcessor& audioProcessor;
    
    juce::Colour color;
    
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> coefficients;
    
    float lineThickness;
    
    bool shouldBeFilled;
};
