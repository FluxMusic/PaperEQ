/*
  ==============================================================================

    OutputComponent.cpp
    Created: 21 May 2024 7:16:48pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "OutputComponent.h"

OutputComponent::OutputComponent(PaperEQAudioProcessor& p)
: audioProcessor(p),
outputGainAttachment(audioProcessor.getAPVTS(), "OutputGain", outputGain)
{
    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);
    
    addAndMakeVisible(outputGain);
    outputGain.setSliderSnapsToMousePosition(false);
}

void OutputComponent::timerCallback()
{
    const auto leftChannelInputValue = audioProcessor.getRMSInputLevel(0);
    const auto rightChannelInputValue = audioProcessor.getRMSInputLevel(1);
    inputMeter.setLevels(leftChannelInputValue, rightChannelInputValue);
    
    const auto leftChannelOutputValue = audioProcessor.getRMSInputLevel(0);
    const auto rightChannelOutputValue = audioProcessor.getRMSInputLevel(1);
    outputMeter.setLevels(leftChannelOutputValue, rightChannelOutputValue);
}

void OutputComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::floralwhite);
}

void OutputComponent::resized()
{
    auto bounds = getLocalBounds();
    auto inputMeterBounds = bounds.removeFromLeft(bounds.getWidth() / 3);
    auto outputMeterBounds = bounds.removeFromRight(bounds.getWidth() / 2);
    
    inputMeter.setBounds(inputMeterBounds);
    outputMeter.setBounds(outputMeterBounds);
    outputGain.setBounds(bounds);
}
