/*
  ==============================================================================

    LowCutBandComponent.cpp
    Created: 21 May 2024 1:54:19pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "LowCutBandComponent.h"

LowCutBandComponent::LowCutBandComponent(PaperEQAudioProcessor& p)
: audioProcessor(p),
freqSlider(*audioProcessor.getAPVTS().getParameter("LowCutFreq"), ""),
slopeSlider(*audioProcessor.getAPVTS().getParameter("LowCutSlope"), ""),
freqSliderAttachment(audioProcessor.getAPVTS(), "LowCutFreq", freqSlider),
slopeSliderAttachment(audioProcessor.getAPVTS(), "LowCutSlope", slopeSlider)
{
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(slopeSlider);
}

void LowCutBandComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::floralwhite);
    
    auto bounds = getLocalBounds().toFloat().reduced(getLocalBounds().getWidth() / 40);
    
    g.setColour(juce::Colours::red);
    g.drawRoundedRectangle(bounds, 5, bounds.getWidth() / 50);
}

void LowCutBandComponent::resized()
{
    auto fullBounds = getLocalBounds();
    
    auto bounds = fullBounds.reduced(fullBounds.getWidth() / 40);
    
    auto bypassBounds = bounds.removeFromTop(bounds.getHeight() / 5);
    bypassButton.setBounds(bypassBounds);
    
    auto freqBounds = bounds.removeFromTop(bounds.getHeight() / 2);
    freqBounds.removeFromLeft(freqBounds.getWidth() / 5);
    freqSlider.setBounds(freqBounds);
    
    bounds.removeFromRight(bounds.getWidth() / 5);
    slopeSlider.setBounds(bounds);
}
