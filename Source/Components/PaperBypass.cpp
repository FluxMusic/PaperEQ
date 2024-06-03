/*
  ==============================================================================

    PaperBypass.cpp
    Created: 3 Jun 2024 9:19:18am
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperBypass.h"

PaperBypass::PaperBypass(juce::String buttonText)
: text(buttonText)
{
    setLookAndFeel(&lnf);
}

PaperBypass::~PaperBypass()
{
    setLookAndFeel(nullptr);
}

void PaperBypass::paint(juce::Graphics& g)
{
    getLookAndFeel().drawButtonBackground(g,
                                          *this,
                                          juce::Colours::floralwhite,
                                          false,
                                          false);
    
    this->getToggleState() ? g.setColour(juce::Colours::floralwhite) : g.setColour(juce::Colours::black);
    g.drawFittedText(text, getLocalBounds(), juce::Justification::centred, 1);
}
