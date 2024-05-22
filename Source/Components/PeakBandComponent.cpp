/*
  ==============================================================================

    PeakBandComponent.cpp
    Created: 21 May 2024 1:17:21pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PeakBandComponent.h"

PeakBandComponent::PeakBandComponent(PaperEQAudioProcessor& p)
: audioProcessor(p),
gainSlider(*audioProcessor.getAPVTS().getParameter("PeakGain"), ""),
freqSlider(*audioProcessor.getAPVTS().getParameter("PeakFreq"), ""),
qSlider(*audioProcessor.getAPVTS().getParameter("PeakQ"), ""),
gainSliderAttachment(audioProcessor.getAPVTS(), "PeakGain", gainSlider),
freqSliderAttachment(audioProcessor.getAPVTS(), "PeakFreq", freqSlider),
qSliderAttachment(audioProcessor.getAPVTS(), "PeakQ", qSlider)
{
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(qSlider);
}

void PeakBandComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::floralwhite);
    
    auto bounds = getLocalBounds().toFloat().reduced(getLocalBounds().getWidth() / 40);
    
    g.setColour(juce::Colours::yellow);
    g.drawRoundedRectangle(bounds, 5, bounds.getWidth() / 50);
}

void PeakBandComponent::resized()
{
    auto fullBounds = getLocalBounds();
    
    auto bounds = fullBounds.reduced(fullBounds.getWidth() / 40);
    
    auto bypassBounds = bounds.removeFromTop(bounds.getHeight() / 5);
    bypassButton.setBounds(bypassBounds);
    
    auto gainBounds = bounds.removeFromBottom(bounds.getHeight() * 0.6);
    gainSlider.setBounds(gainBounds);
    
    auto freqBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
    freqSlider.setBounds(freqBounds);
    
    qSlider.setBounds(bounds);
}
