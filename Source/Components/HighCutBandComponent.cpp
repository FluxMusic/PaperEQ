/*
  ==============================================================================

    HighCutBandComponent.cpp
    Created: 21 May 2024 2:11:59pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "HighCutBandComponent.h"

HighCutBandComponent::HighCutBandComponent(PaperEQAudioProcessor& p)
: audioProcessor(p),
freqSlider(*audioProcessor.getAPVTS().getParameter("HighCutFreq"), ""),
slopeSlider(*audioProcessor.getAPVTS().getParameter("HighCutSlope"), ""),
freqSliderAttachment(audioProcessor.getAPVTS(), "HighCutFreq", freqSlider),
slopeSliderAttachment(audioProcessor.getAPVTS(), "HighCutSlope", slopeSlider)
{
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(slopeSlider);
}

void HighCutBandComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::floralwhite);
}

void HighCutBandComponent::resized()
{
    const auto fullBounds = getLocalBounds();
    auto bounds = fullBounds;
    
    auto bypassBounds = bounds.removeFromTop(bounds.getHeight() / 5);
    bypassButton.setBounds(bypassBounds);
    
    auto freqBounds = bounds.removeFromTop(bounds.getHeight() / 2);
    freqBounds.removeFromRight(freqBounds.getWidth() / 5);
    freqSlider.setBounds(freqBounds);
    
    bounds.removeFromLeft(bounds.getWidth() / 5);
    slopeSlider.setBounds(bounds);
}
