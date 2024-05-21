/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/ResponseCurveComponent.h"
#include "Components/LowCutBandComponent.h"
#include "Components/LowShelfBandComponent.h"
#include "Components/PeakBandComponent.h"
#include "Components/HighShelfBandComponent.h"
#include "Components/HighCutBandComponent.h"

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

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PaperEQAudioProcessor& audioProcessor;
    
    ResponseCurveComponent responseCurveComponent;
    
    LowCutBandComponent lowCutBand;
    LowShelfBandComponent lowShelfBand;
    PeakBandComponent peakBand;
    HighShelfBandComponent highShelfBand;
    HighCutBandComponent highCutBand;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaperEQAudioProcessorEditor)
};
