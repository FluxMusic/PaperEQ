/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PaperEQAudioProcessor::PaperEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    apvts.addParameterListener("LowCutFreq", this);
    apvts.addParameterListener("LowCutSlope", this);
    
    apvts.addParameterListener("LowShelfGain", this);
    apvts.addParameterListener("LowShelfFreq", this);
    apvts.addParameterListener("LowShelfQ", this);
    
    apvts.addParameterListener("PeakGain", this);
    apvts.addParameterListener("PeakFreq", this);
    apvts.addParameterListener("PeakQ", this);
    
    apvts.addParameterListener("HighShelfGain", this);
    apvts.addParameterListener("HighShelfFreq", this);
    apvts.addParameterListener("HighShelfQ", this);
    
    apvts.addParameterListener("HighCutFreq", this);
    apvts.addParameterListener("HighCutSlope", this);
}

PaperEQAudioProcessor::~PaperEQAudioProcessor()
{
    apvts.removeParameterListener("LowCutFreq", this);
    apvts.removeParameterListener("LowCutSlope", this);
    
    apvts.removeParameterListener("LowShelfGain", this);
    apvts.removeParameterListener("LowShelfFreq", this);
    apvts.removeParameterListener("LowShelfQ", this);
    
    apvts.removeParameterListener("PeakGain", this);
    apvts.removeParameterListener("PeakFreq", this);
    apvts.removeParameterListener("PeakQ", this);
    
    apvts.removeParameterListener("HighShelfGain", this);
    apvts.removeParameterListener("HighShelfFreq", this);
    apvts.removeParameterListener("HighShelfQ", this);
    
    apvts.removeParameterListener("HighCutFreq", this);
    apvts.removeParameterListener("HighCutSlope", this);
}

//==============================================================================
const juce::String PaperEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PaperEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PaperEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PaperEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PaperEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PaperEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PaperEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PaperEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PaperEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void PaperEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PaperEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumInputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    leftChain.prepare(spec);
    rightChain.prepare(spec);
}

void PaperEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PaperEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PaperEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    
    auto leftBlock = audioBlock.getSingleChannelBlock(0);
    auto rightBlock = audioBlock.getSingleChannelBlock(1);
    
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);
}

//==============================================================================
bool PaperEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PaperEQAudioProcessor::createEditor()
{
//    return new PaperEQAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PaperEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PaperEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PaperEQAudioProcessor();
}

void PaperEQAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    const auto sampleRate = getSampleRate();
    
    auto parameterSettings = getParameterSettings(apvts);
    
    if (parameterID == "LowCutFreq" || parameterID == "LowCutSlope")
    {
        auto lowCutCoefficientsOrder1 = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(parameterSettings.lowCutFreq, sampleRate, 1);
        auto lowCutCoefficientsOrder2 = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(parameterSettings.lowCutFreq, sampleRate, 2);
        
        leftChain.get<0>().setBypassed<0>(true);
        leftChain.get<0>().setBypassed<1>(true);
        leftChain.get<0>().setBypassed<2>(true);
        leftChain.get<0>().setBypassed<3>(true);
        leftChain.get<0>().setBypassed<4>(true);
        
        rightChain.get<0>().setBypassed<0>(true);
        rightChain.get<0>().setBypassed<1>(true);
        rightChain.get<0>().setBypassed<2>(true);
        rightChain.get<0>().setBypassed<3>(true);
        rightChain.get<0>().setBypassed<4>(true);
        
        switch (parameterSettings.lowCutSlope)
        {
            case Slope_48:
            {
                leftChain.get<0>().setBypassed<4>(false);
                rightChain.get<0>().setBypassed<4>(false);
                
                leftChain.get<0>().get<4>().coefficients = lowCutCoefficientsOrder2[0];
                rightChain.get<0>().get<4>().coefficients = lowCutCoefficientsOrder2[0];
            }
            case Slope_36:
            {
                leftChain.get<0>().setBypassed<3>(false);
                rightChain.get<0>().setBypassed<3>(false);
                
                leftChain.get<0>().get<3>().coefficients = lowCutCoefficientsOrder2[0];
                rightChain.get<0>().get<3>().coefficients = lowCutCoefficientsOrder2[0];
            }
            case Slope_24:
            {
                leftChain.get<0>().setBypassed<2>(false);
                rightChain.get<0>().setBypassed<2>(false);
                
                leftChain.get<0>().get<2>().coefficients = lowCutCoefficientsOrder2[0];
                rightChain.get<0>().get<2>().coefficients = lowCutCoefficientsOrder2[0];
            }
            case Slope_12:
            {
                leftChain.get<0>().setBypassed<1>(false);
                rightChain.get<0>().setBypassed<1>(false);
                
                leftChain.get<0>().get<1>().coefficients = lowCutCoefficientsOrder1[0];
                rightChain.get<0>().get<1>().coefficients = lowCutCoefficientsOrder1[0];
            }
            case Slope_6:
            {
                leftChain.get<0>().setBypassed<0>(false);
                rightChain.get<0>().setBypassed<0>(false);
                
                leftChain.get<0>().get<0>().coefficients = lowCutCoefficientsOrder1[0];
                rightChain.get<0>().get<0>().coefficients = lowCutCoefficientsOrder1[0];
            }
        }
    }
    
    if (parameterID == "LowShelfGain" || parameterID == "LowShelfFreq" || parameterID == "LowShelfQ")
    {
        auto lowShelfCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate,
                                                                                        parameterSettings.lowShelfFreq,
                                                                                        parameterSettings.lowShelfQ,
                                                                                        juce::Decibels::decibelsToGain(parameterSettings.lowShelfGain));
        
        leftChain.get<ChainFilters::LowShelf>().coefficients = lowShelfCoefficients;
        rightChain.get<ChainFilters::LowShelf>().coefficients = lowShelfCoefficients;
    }
    
    if (parameterID == "PeakGain" || parameterID == "PeakFreq" || parameterID == "PeakQ")
    {
        auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
                                                            parameterSettings.peakFreq,
                                                            parameterSettings.peakQ,
                                                            juce::Decibels::decibelsToGain(parameterSettings.peakGain));
        
        leftChain.get<ChainFilters::Peak>().coefficients = peakCoefficients;
        rightChain.get<ChainFilters::Peak>().coefficients = peakCoefficients;
    }
    
    if (parameterID == "HighShelfGain" || parameterID == "HighShelfFreq" || parameterID == "HighShelfQ")
    {
        auto highShelfCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate,
                                                                                        parameterSettings.highShelfFreq,
                                                                                        parameterSettings.highShelfQ,
                                                                                        juce::Decibels::decibelsToGain(parameterSettings.highShelfGain));
        
        leftChain.get<ChainFilters::HighShelf>().coefficients = highShelfCoefficients;
        rightChain.get<ChainFilters::HighShelf>().coefficients = highShelfCoefficients;
    }
    
    if (parameterID == "HighCutFreq" || parameterID == "HighCutSlope")
    {
        auto highCutCoefficientsOrder1 = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(parameterSettings.highCutFreq, sampleRate, 1);
        auto highCutCoefficientsOrder2 = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(parameterSettings.highCutFreq, sampleRate, 2);
        
        leftChain.get<4>().setBypassed<0>(true);
        leftChain.get<4>().setBypassed<1>(true);
        leftChain.get<4>().setBypassed<2>(true);
        leftChain.get<4>().setBypassed<3>(true);
        leftChain.get<4>().setBypassed<4>(true);
        
        rightChain.get<4>().setBypassed<0>(true);
        rightChain.get<4>().setBypassed<1>(true);
        rightChain.get<4>().setBypassed<2>(true);
        rightChain.get<4>().setBypassed<3>(true);
        rightChain.get<4>().setBypassed<4>(true);
        
        switch (parameterSettings.highCutSlope)
        {
            case Slope_48:
            {
                leftChain.get<4>().setBypassed<4>(false);
                rightChain.get<4>().setBypassed<4>(false);
                
                leftChain.get<4>().get<4>().coefficients = highCutCoefficientsOrder2[0];
                rightChain.get<4>().get<4>().coefficients = highCutCoefficientsOrder2[0];
            }
            case Slope_36:
            {
                leftChain.get<4>().setBypassed<3>(false);
                rightChain.get<4>().setBypassed<3>(false);
                
                leftChain.get<4>().get<3>().coefficients = highCutCoefficientsOrder2[0];
                rightChain.get<4>().get<3>().coefficients = highCutCoefficientsOrder2[0];
            }
            case Slope_24:
            {
                leftChain.get<4>().setBypassed<2>(false);
                rightChain.get<4>().setBypassed<2>(false);
                
                leftChain.get<4>().get<2>().coefficients = highCutCoefficientsOrder2[0];
                rightChain.get<4>().get<2>().coefficients = highCutCoefficientsOrder2[0];
            }
            case Slope_12:
            {
                leftChain.get<4>().setBypassed<1>(false);
                rightChain.get<4>().setBypassed<1>(false);
                
                leftChain.get<4>().get<1>().coefficients = highCutCoefficientsOrder1[0];
                rightChain.get<4>().get<1>().coefficients = highCutCoefficientsOrder1[0];
            }
            case Slope_6:
            {
                leftChain.get<4>().setBypassed<0>(false);
                rightChain.get<4>().setBypassed<0>(false);
                
                leftChain.get<4>().get<0>().coefficients = highCutCoefficientsOrder1[0];
                rightChain.get<4>().get<0>().coefficients = highCutCoefficientsOrder1[0];
            }
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout PaperEQAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    juce::StringArray stringArray;
    stringArray.add("6 dB/Oct");
    for (int i = 0; i < 4; ++i)
    {
        juce::String str;
        str << 12 + i * 12;
        str << " dB/Oct";
        stringArray.add(str);
    }
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LowCutFreq", 1), "Low Cut Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 20.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("LowCutSlope", 2), "Low Cut Slope", stringArray, 0));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LowShelfGain", 3), "Low Shelf Gain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LowShelfFreq", 4), "Low Shelf Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 200.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LowShelfQ", 5), "Low Shelf Q", juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 1.f), 1.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("PeakGain", 6), "Peak Gain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("PeakFreq", 7), "Peak Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 750.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("PeakQ", 8), "Peak Q", juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 1.f), 1.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("HighShelfGain", 9), "High Shelf Gain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("HighShelfFreq", 10), "High Shelf Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 5000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("HighShelfQ", 11), "High Shelf Q", juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 1.f), 1.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("HighCutFreq", 12), "High Cut Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 20000.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("HighCutSlope", 13), "High Cut Slope", stringArray, 0));
    
    return layout;
}

ParameterSettings PaperEQAudioProcessor::getParameterSettings(juce::AudioProcessorValueTreeState &apvts)
{
    ParameterSettings parameterSettings;
    
    parameterSettings.lowCutFreq = apvts.getRawParameterValue("LowCutFreq")->load();
    parameterSettings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("LowCutSlope")->load());
    
    parameterSettings.lowShelfGain = apvts.getRawParameterValue("LowShelfGain")->load();
    parameterSettings.lowShelfFreq = apvts.getRawParameterValue("LowShelfFreq")->load();
    parameterSettings.lowShelfQ = apvts.getRawParameterValue("LowShelfQ")->load();
    
    parameterSettings.peakGain = apvts.getRawParameterValue("PeakGain")->load();
    parameterSettings.peakFreq = apvts.getRawParameterValue("PeakFreq")->load();
    parameterSettings.peakQ = apvts.getRawParameterValue("PeakQ")->load();
    
    parameterSettings.highShelfGain = apvts.getRawParameterValue("HighShelfGain")->load();
    parameterSettings.highShelfFreq = apvts.getRawParameterValue("HighShelfFreq")->load();
    parameterSettings.highShelfQ = apvts.getRawParameterValue("HighShelfQ")->load();
    
    parameterSettings.highCutFreq = apvts.getRawParameterValue("HighCutFreq")->load();
    parameterSettings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("HighCutSlope")->load());
    
    return parameterSettings;
}
