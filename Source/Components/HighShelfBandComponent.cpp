/*
  ==============================================================================

    HighShelfBandComponent.cpp
    Created: 21 May 2024 2:07:30pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "HighShelfBandComponent.h"

HighShelfBandComponent::HighShelfBandComponent(PaperEQAudioProcessor& p)
: audioProcessor(p),
gainSlider(*audioProcessor.getAPVTS().getParameter("HighShelfGain"), ""),
freqSlider(*audioProcessor.getAPVTS().getParameter("HighShelfFreq"), ""),
qSlider(*audioProcessor.getAPVTS().getParameter("HighShelfQ"), ""),
gainSliderAttachment(audioProcessor.getAPVTS(), "HighShelfGain", gainSlider),
freqSliderAttachment(audioProcessor.getAPVTS(), "HighShelfFreq", freqSlider),
qSliderAttachment(audioProcessor.getAPVTS(), "HighShelfQ", qSlider)
{
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(qSlider);
}

void HighShelfBandComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(getLocalBounds().getWidth() / 40);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::grey) : g.setColour(juce::Colours::floralwhite);
    
    g.fillRect(bounds);
    
    g.setColour(juce::Colours::green);
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
    
    g.drawFittedText("HS", textBounds, juce::Justification::right, 1);
}

void HighShelfBandComponent::resized()
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
    
    auto gainBounds = bounds.removeFromTop(bounds.getHeight() * 0.6);
    gainSlider.setBounds(gainBounds);
    
    auto freqBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
    freqSlider.setBounds(freqBounds);
    
    qSlider.setBounds(bounds);
}
