/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
enum Slope
{
    Slope_6,
    Slope_12,
    Slope_24,
    Slope_36,
    Slope_48
};

struct ParameterSettings
{
    float lowCutFreq { 0 };
    Slope lowCutSlope { Slope::Slope_6 };
    float lowShelfGain { 0 }, lowShelfFreq { 200 }, lowShelfQ { 1 };
    float peakGain { 0 }, peakFreq { 750 }, peakQ { 1 };
    float highShelfGain { 0 }, highShelfFreq { 5000 }, highShelfQ { 1 };
    float highCutFreq { 20000 };
    Slope highCutSlope { Slope::Slope_6 };
    float outputGainDB { 0 };
};

enum ChainFilters
{
    LowCut,
    LowShelf,
    Peak,
    HighShelf,
    HighCut
};
/**
*/
class PaperEQAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
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

    juce::AudioProcessorValueTreeState& getAPVTS() {return apvts;}
    
    float getRMSInputLevel(const int channel);
    float getRMSOutputLevel(const int channel);
    
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> getCoefficients();
    
    template <int chainSegment>
    void setChainSegmentBypass()
    {
        leftChain.setBypassed<chainSegment>(!leftChain.isBypassed<chainSegment>());
        rightChain.setBypassed<chainSegment>(!rightChain.isBypassed<chainSegment>());
    }
    
private:
    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;
    
    using Filter = juce::dsp::IIR::Filter<float>;
    using CutChain = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter, Filter>;
    
    juce::dsp::ProcessorChain<CutChain, Filter, Filter, Filter, CutChain> leftChain, rightChain;
    
    juce::dsp::Gain<float> outputGain;
    
    juce::LinearSmoothedValue<float> inputLevelL, inputLevelR;
    juce::LinearSmoothedValue<float> outputLevelL, outputLevelR;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    ParameterSettings getParameterSettings(juce::AudioProcessorValueTreeState& apvts);
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaperEQAudioProcessor)
};
