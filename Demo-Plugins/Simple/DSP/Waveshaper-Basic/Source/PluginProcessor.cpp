/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveshaperBasicAudioProcessor::WaveshaperBasicAudioProcessor()
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
    treeState.addParameterListener("input", this);
    treeState.addParameterListener("output", this);
}

WaveshaperBasicAudioProcessor::~WaveshaperBasicAudioProcessor()
{
    treeState.removeParameterListener("input", this);
    treeState.removeParameterListener("output", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout WaveshaperBasicAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
        
    auto pInput = std::make_unique<juce::AudioParameterFloat>("input", "Input", -24.0f, 24.0f, 0.0f);
    auto pOutput = std::make_unique<juce::AudioParameterFloat>("output", "Output", -24.0f, 24.0f, 0.0f);

    params.push_back(std::move(pInput));
    params.push_back(std::move(pOutput));
    
    return { params.begin(), params.end() };
    
}

void WaveshaperBasicAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    updateParameters();
}

void WaveshaperBasicAudioProcessor::updateParameters()
{
    inputModule.setGainDecibels(treeState.getRawParameterValue("input")->load());
    outputModule.setGainDecibels(treeState.getRawParameterValue("output")->load());
}

//==============================================================================
const juce::String WaveshaperBasicAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveshaperBasicAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveshaperBasicAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaveshaperBasicAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaveshaperBasicAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveshaperBasicAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaveshaperBasicAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveshaperBasicAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WaveshaperBasicAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaveshaperBasicAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WaveshaperBasicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    inputModule.prepare(spec);
    inputModule.setRampDurationSeconds(0.02);
    
    outputModule.prepare(spec);
    outputModule.setRampDurationSeconds(0.02);
    
    distortionModule.prepare(spec);
    
    limiterModule.prepare(spec);
    limiterModule.setThreshold(0.99);
    limiterModule.setRelease(1.0f);
    updateParameters();
}

void WaveshaperBasicAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveshaperBasicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void WaveshaperBasicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    juce::dsp::AudioBlock<float> block {buffer};
    
    inputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    distortionModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    limiterModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    outputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool WaveshaperBasicAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WaveshaperBasicAudioProcessor::createEditor()
{
    //return new WaveshaperBasicAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void WaveshaperBasicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void WaveshaperBasicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new WaveshaperBasicAudioProcessor();
}
