/*
  ==============================================================================

    LowCutBandComponent.cpp
    Created: 21 May 2024 1:54:19pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "LowCutBandComponent.h"
#include "../PluginEditor.h"

LowCutBandComponent::LowCutBandComponent(PaperEQAudioProcessor& p)
: audioProcessor(p),
freqSlider(*audioProcessor.getAPVTS().getParameter("LowCutFreq"), ""),
slopeSlider(*audioProcessor.getAPVTS().getParameter("LowCutSlope"), ""),
freqSliderAttachment(audioProcessor.getAPVTS(), "LowCutFreq", freqSlider),
slopeSliderAttachment(audioProcessor.getAPVTS(), "LowCutSlope", slopeSlider),
bypassButtonAttachment(audioProcessor.getAPVTS(), "LowCutBypass", bypassButton)
{
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(slopeSlider);
    
    bypassButton.onClick = [this]()
    {
        repaint();
        if (dynamic_cast<PaperEQAudioProcessorEditor*>(getParentComponent()))
            dynamic_cast<PaperEQAudioProcessorEditor*>(getParentComponent())->getResponseCurveDisplay().getDragBand(0).repaint();
    };
}

void LowCutBandComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(getLocalBounds().getWidth() / 40);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::grey) : g.setColour(juce::Colours::floralwhite);
    
    g.fillRect(bounds);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::red.withBrightness(0.7f).withSaturation(0.5f)) : g.setColour(juce::Colours::red.withSaturation(0.5f));
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
    
    if (const auto svg = juce::XmlDocument::parse(BinaryData::IconLowCut_svg))
    {
        const auto drawable = juce::Drawable::createFromSVG(*svg);
        drawable->setTransformToFit(textBounds.reduced(textBounds.getHeight() / 8).toFloat(), juce::RectanglePlacement::xRight);
        drawable->draw(g, 1.f);
    }
}

void LowCutBandComponent::resized()
{
    auto fullBounds = getLocalBounds();
    
    auto bounds = fullBounds.reduced(fullBounds.getWidth() / 40);
    
    auto bypassBounds = bounds.removeFromTop(bounds.getHeight() / 5);
    auto bypassSpace = bypassBounds.getHeight() / 20;
    bypassBounds.removeFromTop(bypassSpace);
    bypassBounds.removeFromLeft(bypassSpace);
    bypassBounds.removeFromRight(bypassBounds.getWidth() / 2);
    bypassBounds.removeFromBottom(bypassBounds.getHeight() / 3);
    bypassButton.setBounds(bypassBounds);
    
    auto freqBounds = bounds.removeFromTop(bounds.getHeight() / 2);
    freqBounds.removeFromLeft(freqBounds.getWidth() / 5);
    freqSlider.setBounds(freqBounds);
    
    bounds.removeFromRight(bounds.getWidth() / 5);
    slopeSlider.setBounds(bounds);
}
