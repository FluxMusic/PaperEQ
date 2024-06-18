/*
  ==============================================================================

    HighCutBandComponent.cpp
    Created: 21 May 2024 2:11:59pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "HighCutBandComponent.h"

HighCutBandComponent::HighCutBandComponent(PaperEQAudioProcessor& p)
: audioProcessor(p),
freqSlider(*audioProcessor.getAPVTS().getParameter("HighCutFreq"), ""),
slopeSlider(*audioProcessor.getAPVTS().getParameter("HighCutSlope"), ""),
freqSliderAttachment(audioProcessor.getAPVTS(), "HighCutFreq", freqSlider),
slopeSliderAttachment(audioProcessor.getAPVTS(), "HighCutSlope", slopeSlider),
bypassButtonAttachment(audioProcessor.getAPVTS(), "HighCutBypass", bypassButton)
{
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(slopeSlider);
    
    bypassButton.onClick = [this]()
    {
        repaint();
    };
}

void HighCutBandComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(getLocalBounds().getWidth() / 40);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::grey) : g.setColour(juce::Colours::floralwhite);
    
    g.fillRect(bounds);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::blueviolet.withBrightness(0.7f).withSaturation(0.5f)) : g.setColour(juce::Colours::blueviolet.withSaturation(0.5f));
    g.drawRoundedRectangle(bounds, 5, bounds.getWidth() / 50);
    
    auto textBounds = bounds.removeFromTop(bounds.getHeight() / 5).toNearestInt();
    textBounds = textBounds.reduced(textBounds.getHeight() / 13);
    auto space = textBounds.getHeight() / 20;
    textBounds.removeFromTop(space);
    textBounds.removeFromRight(space);
    textBounds.removeFromLeft(textBounds.getWidth() / 2);
    textBounds.removeFromBottom(textBounds.getHeight() / 3);
    
    g.setFont(textBounds.getHeight() * 0.66);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::floralwhite) : g.setColour(juce::Colours::black);
    
    if (const auto svg = juce::XmlDocument::parse(BinaryData::IconHighCut_svg))
    {
        const auto drawable = juce::Drawable::createFromSVG(*svg);
        drawable->setTransformToFit(textBounds.reduced(textBounds.getHeight() / 8).toFloat(), juce::RectanglePlacement::xRight);
        drawable->draw(g, 1.f);
    }
}

void HighCutBandComponent::resized()
{
    auto fullBounds = getLocalBounds();
    
    auto bounds = fullBounds.reduced(fullBounds.getWidth() / 40);
    
    auto bypassBounds = bounds.removeFromTop(bounds.getHeight() / 5);
    auto bypassSpace = bypassBounds.getHeight() / 20;
    bypassBounds.removeFromTop(bypassSpace);
    bypassBounds.removeFromLeft(bypassSpace);
    bypassBounds.removeFromRight(bypassBounds.getWidth() / 2);
    bypassBounds.removeFromBottom(bypassBounds.getHeight() / 3);    bypassButton.setBounds(bypassBounds);
    
    auto freqBounds = bounds.removeFromTop(bounds.getHeight() / 2);
    freqBounds.removeFromRight(freqBounds.getWidth() / 5);
    freqSlider.setBounds(freqBounds);
    
    bounds.removeFromLeft(bounds.getWidth() / 5);
    slopeSlider.setBounds(bounds);
}
