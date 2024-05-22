/*
  ==============================================================================

    PaperKnob.cpp
    Created: 5 May 2024 5:41:45pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperKnob.h"

PaperKnob::PaperKnob(juce::RangedAudioParameter& rap, juce::String label)
: juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
param(&rap),
sliderName(label)
{
    setLookAndFeel(&lnf);
}

PaperKnob::~PaperKnob()
{
    setLookAndFeel(nullptr);
    
//    param = nullptr;
}

void PaperKnob::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    juce::String valueString = "";
    
    if (param->getParameterID().equalsIgnoreCase("PreDelay"))
    {
        valueString = static_cast<juce::String>(getValue());
        valueString.append(" ms", 3);
    }
    else if (param->getParameterID().equalsIgnoreCase("Size"))
    {
        auto range = getRange();
        valueString = juce::String::formatted("%.2f", juce::jmap(getValue(), range.getStart(), range.getEnd(), 1.0, 10.0));
        valueString.append(" S", 2);
    }
    else
    {
        valueString = static_cast<juce::String>(getValue() * 100);
        valueString.append(" %", 2);
    }
    
    const auto valueBounds = bounds.removeFromBottom(bounds.getHeight() / 5);
    
    const auto labelBounds = bounds.removeFromTop(bounds.getHeight() / 4);
    
    const auto startAngle = juce::degreesToRadians(180.f + 30.f);
    const auto endAngle = juce::degreesToRadians(180.f - 30.f) + juce::MathConstants<float>::twoPi;
    
    const auto range = getRange();
    
    getLookAndFeel().drawRotarySlider(g,
                                      bounds.getX(),
                                      bounds.getY(),
                                      bounds.getWidth(),
                                      bounds.getHeight(),
                                      juce::jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
                                      startAngle,
                                      endAngle,
                                      *this);
    
    g.setColour(juce::Colours::black);
    g.setFont(valueBounds.getHeight() / 1.3);
    g.drawFittedText(sliderName, labelBounds, juce::Justification::centred, 1);
    
    g.drawFittedText(static_cast<juce::String>(valueString), valueBounds, juce::Justification::centred, 1);
}
