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
    apvts.addParameterListener("LowCutBypass", this);
    
    apvts.addParameterListener("LowShelfGain", this);
    apvts.addParameterListener("LowShelfFreq", this);
    apvts.addParameterListener("LowShelfQ", this);
    apvts.addParameterListener("LowShelfBypass", this);
    
    apvts.addParameterListener("PeakGain", this);
    apvts.addParameterListener("PeakFreq", this);
    apvts.addParameterListener("PeakQ", this);
    apvts.addParameterListener("PeakBypass", this);
    
    apvts.addParameterListener("HighShelfGain", this);
    apvts.addParameterListener("HighShelfFreq", this);
    apvts.addParameterListener("HighShelfQ", this);
    apvts.addParameterListener("HighShelfBypass", this);
    
    apvts.addParameterListener("HighCutFreq", this);
    apvts.addParameterListener("HighCutSlope", this);
    apvts.addParameterListener("HighCutBypass", this);
    
    apvts.addParameterListener("OutputGain", this);
}

PaperEQAudioProcessor::~PaperEQAudioProcessor()
{
    apvts.removeParameterListener("LowCutFreq", this);
    apvts.removeParameterListener("LowCutSlope", this);
    apvts.removeParameterListener("LowCutBypass", this);
    
    apvts.removeParameterListener("LowShelfGain", this);
    apvts.removeParameterListener("LowShelfFreq", this);
    apvts.removeParameterListener("LowShelfQ", this);
    apvts.removeParameterListener("LowShelfBypass", this);
    
    apvts.removeParameterListener("PeakGain", this);
    apvts.removeParameterListener("PeakFreq", this);
    apvts.removeParameterListener("PeakQ", this);
    apvts.removeParameterListener("PeakBypass", this);
    
    apvts.removeParameterListener("HighShelfGain", this);
    apvts.removeParameterListener("HighShelfFreq", this);
    apvts.removeParameterListener("HighShelfQ", this);
    apvts.removeParameterListener("HighShelfBypass", this);
    
    apvts.removeParameterListener("HighCutFreq", this);
    apvts.removeParameterListener("HighCutSlope", this);
    apvts.removeParameterListener("HighCutBypass", this);
    
    apvts.removeParameterListener("OutputGain", this);
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
    auto parameterSettings = getParameterSettings(apvts);
    
    juce::dsp::ProcessSpec spec;
    
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumInputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    leftChain.prepare(spec);
    rightChain.prepare(spec);
    
    outputGain.setRampDurationSeconds(0.01);
    outputGain.setGainDecibels(parameterSettings.outputGainDB);
    outputGain.prepare(spec);
    
    inputLevelL.reset(sampleRate, 1.0);
    inputLevelR.reset(sampleRate, 1.0);
    
    inputLevelL.setCurrentAndTargetValue(-100.f);
    inputLevelR.setCurrentAndTargetValue(-100.f);
    
    outputLevelL.reset(sampleRate, 1.0);
    outputLevelR.reset(sampleRate, 1.0);
    
    outputLevelL.setCurrentAndTargetValue(-100.f);
    outputLevelR.setCurrentAndTargetValue(-100.f);
    
    updateLowCutFilter(parameterSettings, sampleRate);
    updateHighCutFilter(parameterSettings, sampleRate);
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
    
    const auto bufferSize = buffer.getNumSamples();
    
    inputLevelL.skip(bufferSize);
    inputLevelR.skip(bufferSize);
    
    const auto inputRMSValueL = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, bufferSize));
    const auto inputRMSValueR = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, bufferSize));
    
    if (inputRMSValueL < inputLevelL.getCurrentValue())
        inputLevelL.setTargetValue(inputRMSValueL);
    else
        inputLevelL.setCurrentAndTargetValue(inputRMSValueL);
    
    if (inputRMSValueR < inputLevelR.getCurrentValue())
        inputLevelR.setTargetValue(inputRMSValueR);
    else
        inputLevelR.setCurrentAndTargetValue(inputRMSValueR);

    juce::dsp::AudioBlock<float> audioBlock(buffer);
    
    auto leftBlock = audioBlock.getSingleChannelBlock(0);
    auto rightBlock = audioBlock.getSingleChannelBlock(1);
    
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    
    juce::dsp::ProcessContextReplacing<float> stereoContext(audioBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);
    
    outputGain.process(stereoContext);
    
    outputLevelL.skip(bufferSize);
    outputLevelR.skip(bufferSize);
    
    const auto outputRMSValueL = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, bufferSize));
    const auto outputRMSValueR = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, bufferSize));
    
    if (outputRMSValueL < outputLevelL.getCurrentValue())
        outputLevelL.setTargetValue(outputRMSValueL);
    else
        outputLevelL.setCurrentAndTargetValue(outputRMSValueL);
    
    if (outputRMSValueR < outputLevelR.getCurrentValue())
        outputLevelR.setTargetValue(outputRMSValueR);
    else
        outputLevelR.setCurrentAndTargetValue(outputRMSValueR);
}

//==============================================================================
bool PaperEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PaperEQAudioProcessor::createEditor()
{
    return new PaperEQAudioProcessorEditor (*this);
//    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PaperEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void PaperEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto state = juce::ValueTree::readFromData(data, sizeInBytes);
    
    if (state.isValid())
    {
        apvts.replaceState(state);
    }
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
    
    if (parameterID == "LowCutFreq" || parameterID == "LowCutSlope" || parameterID == "LowCutBypass")
    {
        updateLowCutFilter(parameterSettings, sampleRate);
    }
    
    if (parameterID == "LowShelfGain" || parameterID == "LowShelfFreq" || parameterID == "LowShelfQ" || parameterID == "LowShelfBypass")
    {
        auto lowShelfCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate,
                                                                                        parameterSettings.lowShelfFreq,
                                                                                        parameterSettings.lowShelfQ,
                                                                                        juce::Decibels::decibelsToGain(parameterSettings.lowShelfGain));
        
        leftChain.get<ChainFilters::LowShelf>().coefficients = lowShelfCoefficients;
        rightChain.get<ChainFilters::LowShelf>().coefficients = lowShelfCoefficients;
        
        leftChain.setBypassed<ChainFilters::LowShelf>(parameterSettings.lowShelfBypass);
        rightChain.setBypassed<ChainFilters::LowShelf>(parameterSettings.lowShelfBypass);
    }
    
    if (parameterID == "PeakGain" || parameterID == "PeakFreq" || parameterID == "PeakQ" || parameterID == "PeakBypass")
    {
        auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
                                                            parameterSettings.peakFreq,
                                                            parameterSettings.peakQ,
                                                            juce::Decibels::decibelsToGain(parameterSettings.peakGain));
        
        leftChain.get<ChainFilters::Peak>().coefficients = peakCoefficients;
        rightChain.get<ChainFilters::Peak>().coefficients = peakCoefficients;
        
        leftChain.setBypassed<ChainFilters::Peak>(parameterSettings.peakBypass);
        rightChain.setBypassed<ChainFilters::Peak>(parameterSettings.peakBypass);
    }
    
    if (parameterID == "HighShelfGain" || parameterID == "HighShelfFreq" || parameterID == "HighShelfQ" || parameterID == "HighShelfBypass")
    {
        auto highShelfCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate,
                                                                                        parameterSettings.highShelfFreq,
                                                                                        parameterSettings.highShelfQ,
                                                                                        juce::Decibels::decibelsToGain(parameterSettings.highShelfGain));
        
        leftChain.get<ChainFilters::HighShelf>().coefficients = highShelfCoefficients;
        rightChain.get<ChainFilters::HighShelf>().coefficients = highShelfCoefficients;
        
        leftChain.setBypassed<ChainFilters::HighShelf>(parameterSettings.highShelfBypass);
        rightChain.setBypassed<ChainFilters::HighShelf>(parameterSettings.highShelfBypass);
    }
    
    if (parameterID == "HighCutFreq" || parameterID == "HighCutSlope" || parameterID == "HighCutBypass")
    {
        updateHighCutFilter(parameterSettings, sampleRate);
    }
    
    if (parameterID == "OutputGain")
    {
        outputGain.setGainDecibels(parameterSettings.outputGainDB);
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
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("LowCutBypass", 3), "Low Cut Bypass", true));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LowShelfGain", 4), "Low Shelf Gain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LowShelfFreq", 5), "Low Shelf Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 200.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LowShelfQ", 6), "Low Shelf Q", juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 1.f), 1.f));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("LowShelfBypass", 7), "Low Shelf Bypass", false));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("PeakGain", 8), "Peak Gain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("PeakFreq", 9), "Peak Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 750.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("PeakQ", 10), "Peak Q", juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 1.f), 1.f));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("PeakBypass", 11), "Peak Bypass", false));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("HighShelfGain", 12), "High Shelf Gain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("HighShelfFreq", 13), "High Shelf Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 5000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("HighShelfQ", 14), "High Shelf Q", juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 1.f), 1.f));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("HighShelfBypass", 15), "High Shelf Bypass", false));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("HighCutFreq", 16), "High Cut Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 20000.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("HighCutSlope", 17), "High Cut Slope", stringArray, 0));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("HighCutBypass", 18), "High Cut Bypass", true));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("OutputGain", 19), "Output Gain", juce::NormalisableRange<float>(-18.f, 18.f, 0.1f, 1.f), 0.f));
    
    return layout;
}

ParameterSettings PaperEQAudioProcessor::getParameterSettings(juce::AudioProcessorValueTreeState &apvts)
{
    ParameterSettings parameterSettings;
    
    parameterSettings.lowCutFreq = apvts.getRawParameterValue("LowCutFreq")->load();
    parameterSettings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("LowCutSlope")->load());
    parameterSettings.lowCutBypass = apvts.getRawParameterValue("LowCutBypass")->load() > 0.5f;
    
    parameterSettings.lowShelfGain = apvts.getRawParameterValue("LowShelfGain")->load();
    parameterSettings.lowShelfFreq = apvts.getRawParameterValue("LowShelfFreq")->load();
    parameterSettings.lowShelfQ = apvts.getRawParameterValue("LowShelfQ")->load();
    parameterSettings.lowShelfBypass = apvts.getRawParameterValue("LowShelfBypass")->load() > 0.5f;
    
    parameterSettings.peakGain = apvts.getRawParameterValue("PeakGain")->load();
    parameterSettings.peakFreq = apvts.getRawParameterValue("PeakFreq")->load();
    parameterSettings.peakQ = apvts.getRawParameterValue("PeakQ")->load();
    parameterSettings.peakBypass = apvts.getRawParameterValue("PeakBypass")->load() > 0.5f;
    
    parameterSettings.highShelfGain = apvts.getRawParameterValue("HighShelfGain")->load();
    parameterSettings.highShelfFreq = apvts.getRawParameterValue("HighShelfFreq")->load();
    parameterSettings.highShelfQ = apvts.getRawParameterValue("HighShelfQ")->load();
    parameterSettings.highShelfBypass = apvts.getRawParameterValue("HighShelfBypass")->load() > 0.5f;
    
    parameterSettings.highCutFreq = apvts.getRawParameterValue("HighCutFreq")->load();
    parameterSettings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("HighCutSlope")->load());
    parameterSettings.highCutBypass = apvts.getRawParameterValue("HighCutBypass")->load() > 0.5f;
    
    parameterSettings.outputGainDB = apvts.getRawParameterValue("OutputGain")->load();
    
    return parameterSettings;
}

float PaperEQAudioProcessor::getRMSInputLevel(const int channel)
{
    if (channel == 0)
        return inputLevelL.getCurrentValue();
    if (channel == 1)
        return inputLevelR.getCurrentValue();
    else
        return 0;
}

float PaperEQAudioProcessor::getRMSOutputLevel(const int channel)
{
    if (channel == 0)
        return outputLevelL.getCurrentValue();
    if (channel == 1)
        return outputLevelR.getCurrentValue();
    else
        return 0;
}

juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> PaperEQAudioProcessor::getCoefficients()
{
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> coefficients;
    
    auto parameterSettings = getParameterSettings(apvts);
    
    if (!parameterSettings.lowCutBypass)
    {
        switch (parameterSettings.lowCutSlope)
        {
            case Slope_48:
                coefficients.add(leftChain.get<0>().get<4>().coefficients);
            case Slope_36:
                coefficients.add(leftChain.get<0>().get<3>().coefficients);
            case Slope_24:
                coefficients.add(leftChain.get<0>().get<2>().coefficients);
            case Slope_12:
                coefficients.add(leftChain.get<0>().get<1>().coefficients);
            case Slope_6:
                coefficients.add(leftChain.get<0>().get<0>().coefficients);
        }
    }
    if (!parameterSettings.lowShelfBypass)
        coefficients.add(leftChain.get<1>().coefficients);
    if (!parameterSettings.peakBypass)
        coefficients.add(leftChain.get<2>().coefficients);
    if (!parameterSettings.highShelfBypass)
        coefficients.add(leftChain.get<3>().coefficients);
    
    if (!parameterSettings.highCutBypass)
    {
        switch (parameterSettings.highCutSlope)
        {
            case Slope_48:
                coefficients.add(leftChain.get<4>().get<4>().coefficients);
            case Slope_36:
                coefficients.add(leftChain.get<4>().get<3>().coefficients);
            case Slope_24:
                coefficients.add(leftChain.get<4>().get<2>().coefficients);
            case Slope_12:
                coefficients.add(leftChain.get<4>().get<1>().coefficients);
            case Slope_6:
                coefficients.add(leftChain.get<4>().get<0>().coefficients);
        }
    }
    
    return coefficients;
}

void PaperEQAudioProcessor::updateLowCutFilter(ParameterSettings& parameterSettings, double sampleRate)
{
    const auto initialCoefficients = new juce::dsp::IIR::Coefficients<float>(1, 1, 1, 1, 1, 1);
    
    auto filterCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(parameterSettings.lowCutFreq, sampleRate, 1);
    
    switch (parameterSettings.lowCutSlope)
    {
        case Slope_12:
            filterCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(parameterSettings.lowCutFreq, sampleRate, 2);
            break;
        case Slope_24:
            filterCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(parameterSettings.lowCutFreq, sampleRate, 4);
            break;
        case Slope_36:
            filterCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(parameterSettings.lowCutFreq, sampleRate, 6);
            break;
        case Slope_48:
            filterCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(parameterSettings.lowCutFreq, sampleRate, 8);
            break;
        default:
            break;
    }
    
    updateLowCutFilterCoefficients<3>(initialCoefficients);
    updateLowCutFilterCoefficients<2>(initialCoefficients);
    updateLowCutFilterCoefficients<1>(initialCoefficients);
    updateLowCutFilterCoefficients<0>(initialCoefficients);
    
    switch (parameterSettings.lowCutSlope)
    {
        case Slope_48:
        {
            updateLowCutFilterCoefficients<3>(filterCoefficients[3]);
        }
        case Slope_36:
        {
            updateLowCutFilterCoefficients<2>(filterCoefficients[2]);
        }
        case Slope_24:
        {
            updateLowCutFilterCoefficients<1>(filterCoefficients[1]);
        }
        case Slope_12:
        {
            updateLowCutFilterCoefficients<0>(filterCoefficients[0]);
        }
        case Slope_6:
        {
            updateLowCutFilterCoefficients<0>(filterCoefficients[0]);
        }
    }
    
    leftChain.setBypassed<ChainFilters::LowCut>(parameterSettings.lowCutBypass);
    rightChain.setBypassed<ChainFilters::LowCut>(parameterSettings.lowCutBypass);
}

void PaperEQAudioProcessor::updateHighCutFilter(ParameterSettings& parameterSettings, double sampleRate)
{
    const auto initialCoefficients = new juce::dsp::IIR::Coefficients<float>(1, 1, 1, 1, 1, 1);
    
    auto filterCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(parameterSettings.highCutFreq, sampleRate, 1);
    
    switch (parameterSettings.highCutSlope)
    {
        case Slope_12:
            filterCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(parameterSettings.highCutFreq, sampleRate, 2);
            break;
        case Slope_24:
            filterCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(parameterSettings.highCutFreq, sampleRate, 4);
            break;
        case Slope_36:
            filterCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(parameterSettings.highCutFreq, sampleRate, 6);
            break;
        case Slope_48:
            filterCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(parameterSettings.highCutFreq, sampleRate, 8);
            break;
        default:
            break;
    }
    
    updateHighCutFilterCoefficients<3>(initialCoefficients);
    updateHighCutFilterCoefficients<2>(initialCoefficients);
    updateHighCutFilterCoefficients<1>(initialCoefficients);
    updateHighCutFilterCoefficients<0>(initialCoefficients);
    
    switch (parameterSettings.highCutSlope)
    {
        case Slope_48:
        {
            updateHighCutFilterCoefficients<3>(filterCoefficients[3]);
        }
        case Slope_36:
        {
            updateHighCutFilterCoefficients<2>(filterCoefficients[2]);
        }
        case Slope_24:
        {
            updateHighCutFilterCoefficients<1>(filterCoefficients[1]);
        }
        case Slope_12:
        {
            updateHighCutFilterCoefficients<0>(filterCoefficients[0]);
        }
        case Slope_6:
        {
            updateHighCutFilterCoefficients<0>(filterCoefficients[0]);
        }
    }
    
    leftChain.setBypassed<ChainFilters::HighCut>(parameterSettings.highCutBypass);
    rightChain.setBypassed<ChainFilters::HighCut>(parameterSettings.highCutBypass);
}

juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> PaperEQAudioProcessor::getLowCutCoefficients()
{
    auto parameterSettings = getParameterSettings(apvts);
    
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> coefficients;
    
    switch (parameterSettings.lowCutSlope)
    {
        case Slope_6:
        {
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<0>().coefficients);
            break;
        }
        case Slope_12:
        {
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<0>().coefficients);
            break;
        }
        case Slope_24:
        {
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<0>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<1>().coefficients);
            break;
        }
        case Slope_36:
        {
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<0>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<1>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<2>().coefficients);
            break;
        }
        case Slope_48:
        {
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<0>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<1>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<2>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::LowCut>().get<3>().coefficients);
            break;
        }
    }
    
    return coefficients;
}

juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> PaperEQAudioProcessor::getLowShelfCoefficients()
{
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> coefficients;
    
    coefficients.add(leftChain.get<ChainFilters::LowShelf>().coefficients);
    
    return coefficients;
}

juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> PaperEQAudioProcessor::getPeakCoefficients()
{
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> coefficients;
    
    coefficients.add(leftChain.get<ChainFilters::Peak>().coefficients);
    
    return coefficients;
}

juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> PaperEQAudioProcessor::getHighShelfCoefficients()
{
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> coefficients;
    
    coefficients.add(leftChain.get<ChainFilters::HighShelf>().coefficients);
    
    return coefficients;
}

juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> PaperEQAudioProcessor::getHighCutCoefficients()
{
    auto parameterSettings = getParameterSettings(apvts);
    
    juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> coefficients;

    switch (parameterSettings.highCutSlope)
    {
        case Slope_6:
        {
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<0>().coefficients);
            break;
        }
        case Slope_12:
        {
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<0>().coefficients);
            break;
        }
        case Slope_24:
        {
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<0>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<1>().coefficients);
            break;
        }
        case Slope_36:
        {
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<0>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<1>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<2>().coefficients);
            break;
        }
        case Slope_48:
        {
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<0>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<1>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<2>().coefficients);
            coefficients.add(leftChain.get<ChainFilters::HighCut>().get<3>().coefficients);
            break;
        }
    }
    
    return coefficients;
}

template <int filterSegment, typename CoefficientType>
void PaperEQAudioProcessor::updateLowCutFilterCoefficients(const CoefficientType& newCoefficients)
{
    leftChain.get<0>().setBypassed<filterSegment>(false);
    rightChain.get<0>().setBypassed<filterSegment>(false);
    
    leftChain.get<0>().get<filterSegment>().coefficients = newCoefficients;
    rightChain.get<0>().get<filterSegment>().coefficients = newCoefficients;
}

template <int filterSegment, typename CoefficientType>
void PaperEQAudioProcessor::updateHighCutFilterCoefficients(const CoefficientType& newCoefficients)
{
    leftChain.get<4>().setBypassed<filterSegment>(false);
    rightChain.get<4>().setBypassed<filterSegment>(false);
    
    leftChain.get<4>().get<filterSegment>().coefficients = newCoefficients;
    rightChain.get<4>().get<filterSegment>().coefficients = newCoefficients;
}
