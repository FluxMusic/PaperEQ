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
    g.fillAll(juce::Colours::floralwhite);
    
    auto bounds = getLocalBounds().toFloat().reduced(getLocalBounds().getWidth() / 40);
    
    g.setColour(juce::Colours::green);
    g.drawRoundedRectangle(bounds, 5, bounds.getWidth() / 50);
}

void HighShelfBandComponent::resized()
{
    auto fullBounds = getLocalBounds();
    
    auto bounds = fullBounds.reduced(fullBounds.getWidth() / 40);
    
    auto bypassBounds = bounds.removeFromTop(bounds.getHeight() / 5);
    bypassButton.setBounds(bypassBounds);
    
    auto gainBounds = bounds.removeFromTop(bounds.getHeight() * 0.6);
    gainSlider.setBounds(gainBounds);
    
    auto freqBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
    freqSlider.setBounds(freqBounds);
    
    qSlider.setBounds(bounds);
}
