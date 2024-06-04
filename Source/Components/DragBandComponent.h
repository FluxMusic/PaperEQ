/*
  ==============================================================================

    DragBandComponent.h
    Created: 9 Apr 2024 1:08:02pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

class DragBandComponent : public juce::Component, juce::Slider::Listener
{
public:
    enum class Axis
    {
        X,
        Y,
        Z
    };
    
    class Thumb : public juce::Component
    {
    public:
        Thumb(juce::Colour color);
        void paint(juce::Graphics& g) override;
        void mouseDown(const juce::MouseEvent& event) override;
        void mouseDrag(const juce::MouseEvent& event) override;
        void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;
        std::function<void(juce::Point<double>)> moveCallback;
        std::function<void(juce::MouseWheelDetails)> wheelCallback;
        
    private:
        juce::ComponentDragger dragger;
        juce::ComponentBoundsConstrainer constrainer;
        
        juce::Colour color;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Thumb);
    };
    
    DragBandComponent(PaperEQAudioProcessor&, int band, juce::Colour color);
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void registerSlider(juce::Slider* slider, Axis axis);
    void deregisterSlider(juce::Slider* slider);
    
    DragBandComponent::Thumb& getThumb() { return thumb; }
    
private:
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> coefficients;
    
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> updateCoefficients(int band);
    
    PaperEQAudioProcessor& audioProcessor;
    
    juce::Colour color;
    
    int band;
    
    void sliderValueChanged(juce::Slider* slider) override;
    
    std::vector<juce::Slider*> xSliders, ySliders, zSliders;
    
    DragBandComponent::Thumb thumb;
    
    static constexpr int thumbSize = 12;
};
