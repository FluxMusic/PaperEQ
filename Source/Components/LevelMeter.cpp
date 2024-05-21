/*
  ==============================================================================

    LevelMeter.cpp
    Created: 21 May 2024 5:51:40pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "LevelMeter.h"

SingleMeter::SingleMeter()
{
}

void SingleMeter::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour(juce::Colours::grey);
    g.fillRoundedRectangle(bounds, 5.f);
    
    g.setColour(juce::Colours::black);
    auto fill = juce::jmap(level, -100.f, 0.f, bounds.getX(), bounds.getWidth());
    g.fillRoundedRectangle(bounds.removeFromBottom(fill), 5.f);
}

LevelMeter::LevelMeter(PaperEQAudioProcessor& p)
: audioProcessor(p)
{
    addAndMakeVisible(meterL);
    addAndMakeVisible(meterR);
}

void LevelMeter::resized()
{
    const auto fullBounds = getLocalBounds();
    auto bounds = fullBounds;
    auto leftMeterBounds = bounds.removeFromLeft(fullBounds.getWidth() * 0.43);
    auto rightMeterBounds = bounds.removeFromRight(fullBounds.getWidth() * 0.43);
}

void LevelMeter::setLevels(float leftChannelValue, float rightChannelValue)
{
    meterL.setLevel(leftChannelValue);
    meterR.setLevel(rightChannelValue);
}
