/*
  ==============================================================================

    PaperOutputGainSlider.cpp
    Created: 30 May 2024 9:47:56pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperOutputGainSlider.h"

PaperOutputGainSlider::PaperOutputGainSlider(juce::RangedAudioParameter& rap)
: param(&rap)
{
    addAndMakeVisible(textBox);
    setLookAndFeel(&lookAndFeel);
    setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
}

PaperOutputGainSlider::~PaperOutputGainSlider()
{
    setLookAndFeel(nullptr);
}

void PaperOutputGainSlider::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toDouble();
    
    auto labelBounds = bounds.removeFromBottom(bounds.getHeight() / 18);
    
    textBox.setBounds(labelBounds.toNearestInt());
    textBox.setJustificationType(juce::Justification::centred);
    
    auto labelText = juce::String::formatted("%.1f", getValue());
    labelText << " dB";
    
    textBox.setText(labelText, juce::dontSendNotification);
    textBox.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::floralwhite);
    textBox.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    
    const auto range = getRange();
    
    const auto minSliderPos = bounds.getHeight() - bounds.getHeight() / 13.5;
    const auto maxSliderPos = bounds.getY() + bounds.getHeight() / 13;
    
    setMouseDragSensitivity(minSliderPos - maxSliderPos);
    
    getLookAndFeel().drawLinearSlider(g,
                                      bounds.getX(),
                                      bounds.getY(),
                                      bounds.getWidth(),
                                      bounds.getHeight(),
                                      juce::jmap(static_cast<double>(getValue()), range.getStart(), range.getEnd(), minSliderPos, maxSliderPos),
                                      minSliderPos,
                                      maxSliderPos,
                                      juce::Slider::SliderStyle::LinearBarVertical,
                                      *this);
}
