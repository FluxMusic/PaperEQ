/*
  ==============================================================================

    PaperBypass.h
    Created: 3 Jun 2024 9:19:18am
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel/PaperLookAndFeel.h"

class PaperBypass : public  juce::ToggleButton
{
public:
    PaperBypass(juce::String buttonText);
    ~PaperBypass();
    
private:
    void paint(juce::Graphics& g) override;
    
    PaperLookAndFeel lnf;
    
    juce::String text;
};
