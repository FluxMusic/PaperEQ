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
    auto fill = juce::jmap(level, -60.f, 0.f, bounds.getY(), bounds.getBottom());
    g.fillRoundedRectangle(bounds.removeFromBottom(fill), 5.f);
}

LevelMeter::LevelMeter()
{
    addAndMakeVisible(meterL);
    addAndMakeVisible(meterR);
}

void LevelMeter::paint(juce::Graphics& g)
{
    meterL.repaint();
    meterR.repaint();
}

void LevelMeter::resized()
{
    const auto fullBounds = getLocalBounds();
    auto bounds = fullBounds;
    auto leftMeterBounds = bounds.removeFromLeft(fullBounds.getWidth() * 0.43);
    auto rightMeterBounds = bounds.removeFromRight(fullBounds.getWidth() * 0.43);
    
    meterL.setBounds(leftMeterBounds);
    meterR.setBounds(rightMeterBounds);
}

void LevelMeter::setLevels(float leftChannelValue, float rightChannelValue)
{
    meterL.setLevel(leftChannelValue);
    meterR.setLevel(rightChannelValue);
}
