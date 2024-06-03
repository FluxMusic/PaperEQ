/*
  ==============================================================================

    ResponseCurveComponent.cpp
    Created: 21 May 2024 12:32:37pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "ResponseCurveComponent.h"

ResponseCurveComponent::ResponseCurveComponent(PaperEQAudioProcessor& p)
: audioProcessor(p)
{
}

void ResponseCurveComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
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
    g.drawVerticalLine(width - 1, top, bottom);
    
    for (auto line : gainLines)
    {
        auto lineY = juce::jmap(line, -18.f, 18.f, top, bottom);
        line == 0 ? g.setColour(juce::Colours::grey) : g.setColour(juce::Colours::grey.withAlpha(0.5f));
        g.drawHorizontalLine(lineY, left, width);
    }
    g.drawHorizontalLine(bottom - 1, left, width);
    
    const auto sampleRate = audioProcessor.getSampleRate();
    
    std::vector<float> lineMagnitudes;
        
    lineMagnitudes.resize(width);
    
    const auto coefficients = audioProcessor.getCoefficients();
    
    for (int i = 0; i <= width; ++i)
    {
        float lineMagnitude = 1.f;
        
        auto freq = juce::mapToLog10(static_cast<float>(i) / width, 20.f, 20000.f);
        
        for (auto coefficient : coefficients)
        {
            lineMagnitude *= coefficient->getMagnitudeForFrequency(freq, sampleRate);
        }
        
        lineMagnitudes[i] = juce::Decibels::gainToDecibels(lineMagnitude);
    }
    
    std::vector<float> fillMagnitudes;
        
    fillMagnitudes.resize(width);
    
    for (int i = 0; i <= width; ++i)
    {
        float fillMagnitude = 1.f;
        
        auto freq = juce::mapToLog10(static_cast<float>(i) / width, 20.f, 20000.f);
        
        for (auto coefficient : coefficients)
        {
            fillMagnitude *= coefficient->getMagnitudeForFrequency(freq, sampleRate);
        }
        
        if (i == 0 || i == width - 1)
            fillMagnitude *= 0;
        
        fillMagnitudes[i] = juce::Decibels::gainToDecibels(fillMagnitude);
    }
    
    juce::Path responseCurve;
    juce::Path responseCurveFill;
    
    auto map = [top, bottom] (float magnitude)
    {
        return juce::jmap(magnitude, 18.f, -18.f, top, bottom);
    };
    
    responseCurve.startNewSubPath(left, map(lineMagnitudes.front()));
    responseCurveFill.startNewSubPath(left, map(fillMagnitudes.front()));
    
    for (size_t i = 1; i < lineMagnitudes.size(); ++i)
    {
        responseCurve.lineTo(left + i, map(lineMagnitudes[i]));
    }
    for (size_t i = 0; i < fillMagnitudes.size(); ++i)
    {
        responseCurveFill.lineTo(left + i, map(fillMagnitudes[i]));
    }
    
    g.setColour(juce::Colours::black);
    g.strokePath(responseCurve, juce::PathStrokeType(bottom / 150));
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.fillPath(responseCurveFill);
}
