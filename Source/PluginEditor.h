/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/ResponseCurveDisplay.h"
#include "Components/LowCutBandComponent.h"
#include "Components/LowShelfBandComponent.h"
#include "Components/PeakBandComponent.h"
#include "Components/HighShelfBandComponent.h"
#include "Components/HighCutBandComponent.h"
#include "Components/OutputComponent.h"
#include "Components/DragBandComponent.h"

//==============================================================================
/**
*/
class PaperEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PaperEQAudioProcessorEditor (PaperEQAudioProcessor&);
    ~PaperEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    ResponseCurveDisplay& getResponseCurveDisplay() { return responseCurveDisplay; }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PaperEQAudioProcessor& audioProcessor;
    
    ResponseCurveDisplay responseCurveDisplay;
    
    LowCutBandComponent lowCutBand;
    LowShelfBandComponent lowShelfBand;
    PeakBandComponent peakBand;
    HighShelfBandComponent highShelfBand;
    HighCutBandComponent highCutBand;
    
    OutputComponent output;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaperEQAudioProcessorEditor)
};
