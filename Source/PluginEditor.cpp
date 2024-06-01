/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PaperEQAudioProcessorEditor::PaperEQAudioProcessorEditor (PaperEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
responseCurveDisplay(p),
lowCutBand(p),
lowShelfBand(p),
peakBand(p),
highShelfBand(p),
highCutBand(p),
output(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(responseCurveDisplay);
    
    addAndMakeVisible(lowCutBand);
    addAndMakeVisible(lowShelfBand);
    addAndMakeVisible(peakBand);
    addAndMakeVisible(highShelfBand);
    addAndMakeVisible(highCutBand);
    
    addAndMakeVisible(output);
    
    setSize (800, 450);
    setResizable(true, true);
    setResizeLimits(800, 450, 1600, 900);
    getConstrainer()->setFixedAspectRatio(1.78);
}

PaperEQAudioProcessorEditor::~PaperEQAudioProcessorEditor()
{
}

//==============================================================================
void PaperEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::floralwhite);
    
    const auto fullBounds = getLocalBounds();
    auto bounds = fullBounds;
    
    auto meterBounds = bounds.removeFromRight(bounds.getWidth() / 4);
//    g.setColour(juce::Colours::purple);
//    g.fillRect(meterBounds);
    
    auto labelBounds = meterBounds.removeFromTop(fullBounds.getHeight() / 12);
    
    auto responseCurveBounds = bounds.removeFromTop(bounds.getHeight() / 1.7);
//    g.setColour(juce::Colours::black);
//    g.fillRect(responseCurveBounds);
    
    auto lowCutBounds = bounds. removeFromLeft(bounds.getWidth() / 5);
    g.setColour(juce::Colours::red);
    g.fillRect(lowCutBounds);
    
    auto lowShelfBounds = bounds.removeFromLeft(bounds.getWidth() / 4);
    g.setColour(juce::Colours::orange);
    g.fillRect(lowShelfBounds);
    
    auto peakBounds = bounds.removeFromLeft(bounds.getWidth() / 3);
//    g.setColour(juce::Colours::yellow);
//    g.fillRect(peakBounds);
    
    auto highShelfBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
//    g.setColour(juce::Colours::green);
//    g.fillRect(highShelfBounds);
    
//    g.setColour(juce::Colours::blue);
//    g.fillRect(bounds);

    g.setColour (juce::Colours::black);
    g.setFont (labelBounds.getHeight() / 1.5);
    g.drawFittedText ("Paper EQ", labelBounds, juce::Justification::centred, 1);
}

void PaperEQAudioProcessorEditor::resized()
{
    const auto fullBounds = getLocalBounds();
    auto bounds = fullBounds;
    
    auto meterBounds = bounds.removeFromRight(bounds.getWidth() / 4);
    meterBounds.removeFromTop(fullBounds.getHeight() / 12);
    meterBounds.removeFromLeft(fullBounds.getWidth() / 50);
    meterBounds.removeFromRight(fullBounds.getWidth() / 50);
    output.setBounds(meterBounds);
    
    auto responseCurveBounds = bounds.removeFromTop(bounds.getHeight() / 1.7);
    responseCurveDisplay.setBounds(responseCurveBounds);
    
    auto lowCutBounds = bounds. removeFromLeft(bounds.getWidth() / 5);
    lowCutBand.setBounds(lowCutBounds);
    
    auto lowShelfBounds = bounds.removeFromLeft(bounds.getWidth() / 4);
    lowShelfBand.setBounds(lowShelfBounds);
    
    auto peakBounds = bounds.removeFromLeft(bounds.getWidth() / 3);
    peakBand.setBounds(peakBounds);
    
    auto highShelfBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
    highShelfBand.setBounds(highShelfBounds);
    
    highCutBand.setBounds(bounds);
}
