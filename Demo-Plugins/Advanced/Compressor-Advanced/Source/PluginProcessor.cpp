/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAdvancedAudioProcessor::CompressorAdvancedAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener(inputID, this);
    treeState.addParameterListener(threshID, this);
    treeState.addParameterListener(ratioID, this);
    treeState.addParameterListener(attackID, this);
    treeState.addParameterListener(releaseID, this);
    treeState.addParameterListener(LThreshID, this);
    treeState.addParameterListener(LReleaseID, this);
    treeState.addParameterListener(outputID, this);
}

CompressorAdvancedAudioProcessor::~CompressorAdvancedAudioProcessor()
{
    treeState.removeParameterListener(inputID, this);
    treeState.removeParameterListener(threshID, this);
    treeState.removeParameterListener(ratioID, this);
    treeState.removeParameterListener(attackID, this);
    treeState.removeParameterListener(releaseID, this);
    treeState.removeParameterListener(LThreshID, this);
    treeState.removeParameterListener(LReleaseID, this);
    treeState.removeParameterListener(outputID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorAdvancedAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
        
    juce::NormalisableRange<float> attackRange = juce::NormalisableRange<float>(0.0f, 200.0f, 1.0f);
    attackRange.setSkewForCentre(50.0f);
    
    juce::NormalisableRange<float> releaseRange = juce::NormalisableRange<float>(5.0f, 5000.0f, 1.0f);
    releaseRange.setSkewForCentre(160.0f);
    
    juce::NormalisableRange<float> LReleaseRange = juce::NormalisableRange<float>(1.0f, 1000.0f, 1.0f);
    LReleaseRange.setSkewForCentre(250.0f);
    
    auto pInput = std::make_unique<juce::AudioParameterFloat>(inputID, inputName, -60.0f, 24.0f, 0.0f);
    auto pThresh = std::make_unique<juce::AudioParameterFloat>(threshID, threshName, -60.0f, 10.0f, 0.0f);
    auto pRatio = std::make_unique<juce::AudioParameterFloat>(ratioID, ratioName, 1.0f, 20.0f, 1.0f);
    auto pAttack = std::make_unique<juce::AudioParameterFloat>(attackID, attackName, attackRange, 50.0f);
    auto pRelease = std::make_unique<juce::AudioParameterFloat>(releaseID, releaseName, releaseRange, 160.0f);
    auto pLThresh = std::make_unique<juce::AudioParameterFloat>(LThreshID, LThreshName, -60.0f, 0.0f, 0.0f);
    auto pLRelease = std::make_unique<juce::AudioParameterFloat>(LReleaseID, LReleaseName, LReleaseRange, 250.0f);
    auto pOutput = std::make_unique<juce::AudioParameterFloat>(outputID, outputName, -60.0f, 24.0f, 0.0f);

    params.push_back(std::move(pInput));
    params.push_back(std::move(pThresh));
    params.push_back(std::move(pRatio));
    params.push_back(std::move(pAttack));
    params.push_back(std::move(pRelease));
    params.push_back(std::move(pLThresh));
    params.push_back(std::move(pLRelease));
    params.push_back(std::move(pOutput));
    
    return { params.begin(), params.end() };
}

void CompressorAdvancedAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    updateParameters();
}

void CompressorAdvancedAudioProcessor::updateParameters()
{
    // Update all DSP module parameters
    inputModule.setGainDecibels(treeState.getRawParameterValue(inputID)->load());
    compressorModule.setThreshold(treeState.getRawParameterValue(threshID)->load());
    compressorModule.setRatio(treeState.getRawParameterValue(ratioID)->load());
    compressorModule.setAttack(treeState.getRawParameterValue(attackID)->load());
    compressorModule.setRelease(treeState.getRawParameterValue(releaseID)->load());
    limiterModule.setThreshold(treeState.getRawParameterValue(LThreshID)->load());
    limiterModule.setRelease(treeState.getRawParameterValue(LReleaseID)->load());
    outputModule.setGainDecibels(treeState.getRawParameterValue(outputID)->load());
}

//==============================================================================
const juce::String CompressorAdvancedAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CompressorAdvancedAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAdvancedAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAdvancedAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorAdvancedAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorAdvancedAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorAdvancedAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorAdvancedAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CompressorAdvancedAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorAdvancedAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CompressorAdvancedAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    // Prepare DSP modules for processing
    inputModule.prepare(spec);
    inputModule.setRampDurationSeconds(0.02);
    outputModule.setRampDurationSeconds(0.02);
    outputModule.prepare(spec);
    compressorModule.prepare(spec);
    limiterModule.prepare(spec);
    
    updateParameters();
}

void CompressorAdvancedAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAdvancedAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void CompressorAdvancedAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> block {buffer};
    
    // Process DSP modules
    inputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    compressorModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    limiterModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    outputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool CompressorAdvancedAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CompressorAdvancedAudioProcessor::createEditor()
{
    return new CompressorAdvancedAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void CompressorAdvancedAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void CompressorAdvancedAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    
    if (tree.isValid())
    {
        treeState.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAdvancedAudioProcessor();
}
