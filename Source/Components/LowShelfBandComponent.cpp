/*
  ==============================================================================

    LowShelfComponent.cpp
    Created: 21 May 2024 2:02:54pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "LowShelfBandComponent.h"

LowShelfBandComponent::LowShelfBandComponent(PaperEQAudioProcessor& p)
: audioProcessor(p),
gainSlider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
freqSlider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
qSlider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
gainSliderAttachment(audioProcessor.getAPVTS(), "LowShelfGain", gainSlider),
freqSliderAttachment(audioProcessor.getAPVTS(), "LowShelfFreq", freqSlider),
qSliderAttachment(audioProcessor.getAPVTS(), "LowShelfQ", qSlider)
{
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(qSlider);
}

void LowShelfBandComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::floralwhite);
}

void LowShelfBandComponent::resized()
{
    const auto fullBounds = getLocalBounds();
    auto bounds = fullBounds;
    
    auto bypassBounds = bounds.removeFromTop(bounds.getHeight() / 5);
    bypassButton.setBounds(bypassBounds);
    
    auto gainBounds = bounds.removeFromTop(bounds.getHeight() * 0.6);
    gainSlider.setBounds(gainBounds);
    
    auto freqBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
    freqSlider.setBounds(freqBounds);
    
    qSlider.setBounds(bounds);
}
