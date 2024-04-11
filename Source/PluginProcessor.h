/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
struct ParameterSettings
{
    float gain { 0 }, freq { 750 }, q { 1 };
};
/**
*/
class PaperEQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PaperEQAudioProcessor();
    ~PaperEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;
    
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>> leftChain, rightChain;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    ParameterSettings getParameterSettings(juce::AudioProcessorValueTreeState& apvts);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaperEQAudioProcessor)
};
