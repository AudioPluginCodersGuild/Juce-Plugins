/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainAdvancedAudioProcessor::GainAdvancedAudioProcessor()
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
    treeState.addParameterListener(gainID, this);
}

GainAdvancedAudioProcessor::~GainAdvancedAudioProcessor()
{
    treeState.removeParameterListener(gainID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout GainAdvancedAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
        
    auto pGain = std::make_unique<juce::AudioParameterFloat>(gainID, gainName, -60.0f, 24.0f, 0.0f);
    
    params.push_back(std::move(pGain));
    
    return { params.begin(), params.end() };
}

void GainAdvancedAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    gainProcessor.setGainDecibels(treeState.getRawParameterValue(gainID)->load());
}

//==============================================================================
const juce::String GainAdvancedAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainAdvancedAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainAdvancedAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainAdvancedAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainAdvancedAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainAdvancedAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GainAdvancedAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainAdvancedAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GainAdvancedAudioProcessor::getProgramName (int index)
{
    return {};
}

void GainAdvancedAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GainAdvancedAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    gainProcessor.prepare(spec);
    gainProcessor.setRampDurationSeconds(0.02);
    gainProcessor.setGainDecibels(treeState.getRawParameterValue(gainID)->load());
}

void GainAdvancedAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainAdvancedAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GainAdvancedAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> audioBlock {buffer};
    gainProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

//==============================================================================
bool GainAdvancedAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GainAdvancedAudioProcessor::createEditor()
{
    return new GainAdvancedAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void GainAdvancedAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GainAdvancedAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainAdvancedAudioProcessor();
}
