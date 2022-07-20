/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAdvancedAudioProcessor::DistortionAdvancedAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, _treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    _treeState.addParameterListener(inputID, this);
    _treeState.addParameterListener(mixID, this);
    _treeState.addParameterListener(outputID, this);
}

DistortionAdvancedAudioProcessor::~DistortionAdvancedAudioProcessor()
{
    _treeState.removeParameterListener(inputID, this);
    _treeState.removeParameterListener(mixID, this);
    _treeState.removeParameterListener(outputID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout DistortionAdvancedAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
        
    auto pDrive = std::make_unique<juce::AudioParameterFloat>(inputID, inputName, 0.0f, 24.0f, 0.0f);
    auto pMix = std::make_unique<juce::AudioParameterFloat>(mixID, mixName, 0.0f, 1.0f, 1.0f);
    auto pOutput = std::make_unique<juce::AudioParameterFloat>(outputID, outputName, -24.0f, 24.0f, 0.0f);
    
    params.push_back(std::move(pDrive));
    params.push_back(std::move(pMix));
    params.push_back(std::move(pOutput));
    
    return { params.begin(), params.end() };
}

void DistortionAdvancedAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    updateParameters();
}

void DistortionAdvancedAudioProcessor::updateParameters()
{
    _distortionModule.setDrive(_treeState.getRawParameterValue(inputID)->load());
    _distortionModule.setMix(_treeState.getRawParameterValue(mixID)->load());
    _distortionModule.setOutput(_treeState.getRawParameterValue(outputID)->load());
}

//==============================================================================
const juce::String DistortionAdvancedAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionAdvancedAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAdvancedAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAdvancedAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionAdvancedAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionAdvancedAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionAdvancedAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionAdvancedAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistortionAdvancedAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionAdvancedAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionAdvancedAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    _distortionModule.prepare(spec);
    updateParameters();
}

void DistortionAdvancedAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionAdvancedAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DistortionAdvancedAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> block {buffer};
    _distortionModule.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool DistortionAdvancedAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionAdvancedAudioProcessor::createEditor()
{
    //return new DistortionAdvancedAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void DistortionAdvancedAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DistortionAdvancedAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionAdvancedAudioProcessor();
}
