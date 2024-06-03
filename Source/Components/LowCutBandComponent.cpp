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
    auto bounds = getLocalBounds().toFloat().reduced(getLocalBounds().getWidth() / 40);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::grey) : g.setColour(juce::Colours::floralwhite);
    
    g.fillRect(bounds);
    
    g.setColour(juce::Colours::red);
    g.drawRoundedRectangle(bounds, 5, bounds.getWidth() / 50);
    
    auto textBounds = bounds.removeFromTop(bounds.getHeight() / 5).toNearestInt();
    textBounds = textBounds.reduced(textBounds.getHeight() / 13);
    auto space = textBounds.getHeight() / 20;
    textBounds.removeFromTop(space);
    textBounds.removeFromRight(space);
    textBounds.removeFromLeft(textBounds.getWidth() / 2);
    textBounds.removeFromBottom(textBounds.getHeight() / 3);
    
    g.setFont(textBounds.getHeight() * 0.66);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::floralwhite) : g.setColour(juce::Colours::black);
    
    g.drawFittedText("LC", textBounds, juce::Justification::right, 1);
}

void LowCutBandComponent::resized()
{
    auto fullBounds = getLocalBounds();
    
    auto bounds = fullBounds.reduced(fullBounds.getWidth() / 40);
    
    auto bypassBounds = bounds.removeFromTop(bounds.getHeight() / 5);
    auto bypassSpace = bypassBounds.getHeight() / 20;
    bypassBounds.removeFromTop(bypassSpace);
    bypassBounds.removeFromLeft(bypassSpace);
    bypassBounds.removeFromRight(bypassBounds.getWidth() / 2);
    bypassBounds.removeFromBottom(bypassBounds.getHeight() / 3);
    bypassButton.setBounds(bypassBounds);
    
    auto freqBounds = bounds.removeFromTop(bounds.getHeight() / 2);
    freqBounds.removeFromLeft(freqBounds.getWidth() / 5);
    freqSlider.setBounds(freqBounds);
    
    bounds.removeFromRight(bounds.getWidth() / 5);
    slopeSlider.setBounds(bounds);
}
