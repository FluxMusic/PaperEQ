/*
  ==============================================================================

    ResponseCurve.cpp
    Created: 9 Apr 2024 2:02:10pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "ResponseCurve.h"

ResponseCurve::ResponseCurve(PaperEQAudioProcessor& p)
: audioProcessor(p)
{
    setInterceptsMouseClicks(false, false);
}

void ResponseCurve::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds().toFloat();
    
    const auto left = bounds.getX();
    const auto width = bounds.getWidth();
    const auto top = bounds.getY();
    const auto bottom = bounds.getHeight();
    
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
