/*
  ==============================================================================

    ResponseCurveDisplay.cpp
    Created: 30 May 2024 2:16:35pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "ResponseCurveDisplay.h"

ResponseCurveDisplay::ResponseCurveDisplay(PaperEQAudioProcessor& p)
: audioProcessor(p),
lowCutDragBand(p, 0, juce::Colours::red.withSaturation(0.5f)),
lowShelfDragBand(p, 1, juce::Colours::orange.withSaturation(0.7f)),
peakDragBand(p, 2, juce::Colours::limegreen.withSaturation(0.4f)),
highShelfDragBand(p, 3, juce::Colours::cornflowerblue.withSaturation(0.5f)),
highCutDragBand(p, 4, juce::Colours::blueviolet.withSaturation(0.5f)),
responseCurveComponent(p),
responseCurve(p)
{
    addAndMakeVisible(responseCurveComponent);
    
    addAndMakeVisible(lowCutDragBand);
    addAndMakeVisible(lowShelfDragBand);
    addAndMakeVisible(peakDragBand);
    addAndMakeVisible(highShelfDragBand);
    addAndMakeVisible(highCutDragBand);
    
    addAndMakeVisible(zerodBLine);
    
    addAndMakeVisible(responseCurve);
}

void ResponseCurveDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    auto gainBounds = bounds.removeFromRight(bounds.getWidth() / 23);
    
    auto freqBounds = bounds.removeFromBottom(bounds.getHeight() / 15);
    
    const auto fontHeight = freqBounds.getHeight() * 0.8;
    
    g.setColour(juce::Colours::floralwhite);
    g.fillRect(gainBounds);
    g.fillRect(freqBounds);
    
    const auto left = bounds.getX();
    const auto width = bounds.getWidth();
    const auto top = bounds.getY();
    const auto bottom = bounds.getHeight();
    
    juce::Array<float> freqDisplay
    {
        50, 100,
        200, 500, 1000,
        2000, 5000, 10000
    };
    
    juce::Array<float> gainDisplay
    {
        15, 12, 9, 6, 3, 0, -3, -6, -9, -12, -15
    };
    
    g.setFont(fontHeight);
    g.setColour(juce::Colours::grey.withAlpha(0.8f));
    
    for (auto display : freqDisplay)
    {
        auto lineX = left + width * juce::mapFromLog10(display, 20.f, 20000.f);
        
        juce::String text;
        
        if (display > 999.f)
        {
            display /= 1000;
            text = static_cast<juce::String>(display);
            text << "k";
        }
        else
        {
            text = static_cast<juce::String>(display);
        }
        
        auto rectWidth = g.getCurrentFont().getStringWidth(text);
        juce::Rectangle<float> area(rectWidth, fontHeight);
        
        area.setCentre(lineX, freqBounds.getCentreY());
        
        g.drawText(text, area, juce::Justification::centred);
    }
    
    for (auto display : gainDisplay)
    {
        auto lineY = juce::jmap(display, 18.f, -18.f, top, bottom);
        display == 0 ? g.setColour(juce::Colours::grey) : g.setColour(juce::Colours::grey.withAlpha(0.8f));
        
        juce::String text;
        
        if (display > 0)
            text << "+";
        
        text << static_cast<juce::String>(display);
        
        auto rectWidth = g.getCurrentFont().getStringWidth(text);
        juce::Rectangle<float> area(rectWidth, fontHeight);
        
        area.setCentre(gainBounds.getCentreX(), lineY);
        
        g.drawText(text, area, juce::Justification::centred);
    }
}

void ResponseCurveDisplay::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromRight(bounds.getWidth() / 23);
    bounds.removeFromBottom(bounds.getHeight() / 15);
    
    responseCurveComponent.setBounds(bounds);
    lowCutDragBand.setBounds(bounds);
    lowShelfDragBand.setBounds(bounds);
    peakDragBand.setBounds(bounds);
    highShelfDragBand.setBounds(bounds);
    highCutDragBand.setBounds(bounds);
    
    zerodBLine.setBounds(bounds);
    responseCurve.setBounds(bounds);
}

DragBandComponent& ResponseCurveDisplay::getDragBand(int dragBand)
{
    switch (dragBand)
    {
        case 0:
            return lowCutDragBand;
            break;
        case 1:
            return lowShelfDragBand;
            break;
        case 2:
            return peakDragBand;
            break;
        case 3:
            return highShelfDragBand;
            break;
        default:
            return highCutDragBand;
            break;
    }
}
