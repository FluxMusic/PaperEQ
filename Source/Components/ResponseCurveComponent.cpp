/*
  ==============================================================================

    ResponseCurveComponent.cpp
    Created: 21 May 2024 12:32:37pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "ResponseCurveComponent.h"

ResponseCurveComponent::ResponseCurveComponent()
{
}

void ResponseCurveComponent::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    g.setColour(juce::Colours::floralwhite);
    g.fillRect(bounds);
    
    const auto left = bounds.getX();
    const auto width = bounds.getWidth();
    const auto top = bounds.getY();
    const auto bottom = bounds.getHeight();
    
    juce::Array<float> freqLines
    {
        20, 30, 40, 50, 60, 70, 80, 90, 100,
        200, 300, 400, 500, 600, 700, 800, 900, 1000,
        2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000,
        20000
    };
    
    juce::Array<float> gainLines
    {
        18, 15, 12, 9, 6, 3, 0,
        -3, -6, -9, -12, -15, -18
    };
    
    g.setColour(juce::Colours::grey.withAlpha(0.5f));
    
    for (auto line : freqLines)
    {
        auto lineX = left + width * juce::mapFromLog10(line, 20.f, 20000.f);
        g.drawVerticalLine(lineX, top, bottom);
    }
    
    for (auto line : gainLines)
    {
        auto lineY = juce::jmap(line, -18.f, 18.f, top, bottom);
        line == 0 ? g.setColour(juce::Colours::grey) : g.setColour(juce::Colours::grey.withAlpha(0.5f));
        g.drawHorizontalLine(lineY, left, width);
    }
    
    //TODO: get coefficients, draw response curve
}
