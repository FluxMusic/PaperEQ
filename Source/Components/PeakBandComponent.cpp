/*
  ==============================================================================

    PeakBandComponent.cpp
    Created: 21 May 2024 1:17:21pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PeakBandComponent.h"

PeakBandComponent::PeakBandComponent(PaperEQAudioProcessor& p)
: audioProcessor(p),
gainSlider(*audioProcessor.getAPVTS().getParameter("PeakGain"), ""),
freqSlider(*audioProcessor.getAPVTS().getParameter("PeakFreq"), ""),
qSlider(*audioProcessor.getAPVTS().getParameter("PeakQ"), ""),
gainSliderAttachment(audioProcessor.getAPVTS(), "PeakGain", gainSlider),
freqSliderAttachment(audioProcessor.getAPVTS(), "PeakFreq", freqSlider),
qSliderAttachment(audioProcessor.getAPVTS(), "PeakQ", qSlider),
bypassButtonAttachment(audioProcessor.getAPVTS(), "PeakBypass", bypassButton)
{
    addAndMakeVisible(bypassButton);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(qSlider);
    
    bypassButton.onClick = [this]()
    {
        repaint();
    };
}

void PeakBandComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(getLocalBounds().getWidth() / 40);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::grey) : g.setColour(juce::Colours::floralwhite);
    
    g.fillRect(bounds);
    
    bypassButton.getToggleState() ? g.setColour(juce::Colours::limegreen.withBrightness(0.7f).withSaturation(0.4f)) : g.setColour(juce::Colours::limegreen.withSaturation(0.4f));
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
    
    if (const auto svg = juce::XmlDocument::parse(BinaryData::IconPeak_svg))
    {
        const auto drawable = juce::Drawable::createFromSVG(*svg);
        drawable->setTransformToFit(textBounds.reduced(textBounds.getHeight() / 8).toFloat(), juce::RectanglePlacement::xRight);
        drawable->draw(g, 1.f);
    }
}

void PeakBandComponent::resized()
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
    
    auto gainBounds = bounds.removeFromBottom(bounds.getHeight() * 0.6);
    gainSlider.setBounds(gainBounds);
    
    auto freqBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
    freqSlider.setBounds(freqBounds);
    
    qSlider.setBounds(bounds);
}
