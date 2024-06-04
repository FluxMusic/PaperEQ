/*
  ==============================================================================

    ResponseCurveDisplay.h
    Created: 30 May 2024 2:16:35pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "ResponseCurveComponent.h"
#include "DragBandComponent.h"
#include "0dBLine.h"
#include "ResponseCurve.h"

class ResponseCurveDisplay : public juce::Component
{
public:
    ResponseCurveDisplay(PaperEQAudioProcessor& p);
    
    DragBandComponent& getDragBand(int dragBand);
    
private:
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    PaperEQAudioProcessor& audioProcessor;
    
    DragBandComponent lowCutDragBand, lowShelfDragBand, peakDragBand, highShelfDragBand, highCutDragBand;
    
    ResponseCurveComponent responseCurveComponent;
    
    ZerodBLine zerodBLine;
    
    ResponseCurve responseCurve;
};
