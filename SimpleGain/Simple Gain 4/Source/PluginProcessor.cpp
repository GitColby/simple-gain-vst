#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

SimpleGainAudioProcessor::SimpleGainAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput ("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr)
{
    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        "gain", "Gain", 0.0f, 1.0f, 0.5f));

    parameters.createAndAddParameter(std::make_unique<juce::AudioParameterBool>(
        "bypass", "Bypass", false));

    parameters.state = juce::ValueTree("params");
}

SimpleGainAudioProcessor::~SimpleGainAudioProcessor() {}

const juce::String SimpleGainAudioProcessor::getName() const { return JucePlugin_Name; }
bool SimpleGainAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}
bool SimpleGainAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}
bool SimpleGainAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}
double SimpleGainAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int SimpleGainAudioProcessor::getNumPrograms() { return 1; }
int SimpleGainAudioProcessor::getCurrentProgram() { return 0; }
void SimpleGainAudioProcessor::setCurrentProgram(int) {}
const juce::String SimpleGainAudioProcessor::getProgramName(int) { return {}; }
void SimpleGainAudioProcessor::changeProgramName(int, const juce::String&) {}

void SimpleGainAudioProcessor::prepareToPlay(double, int) {}
void SimpleGainAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleGainAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void SimpleGainAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    bool bypass = *parameters.getRawParameterValue("bypass") > 0.5f;
    if (bypass)
        return; // Bypass processing: skip

    float gain = *parameters.getRawParameterValue("gain");

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        buffer.applyGain(channel, 0, buffer.getNumSamples(), gain);
    }

    // Clear extra output channels
    for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
    {
        buffer.clear(channel, 0, buffer.getNumSamples());
    }
}

bool SimpleGainAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* SimpleGainAudioProcessor::createEditor()
{
    return new SimpleGainAudioProcessorEditor(*this);
}

void SimpleGainAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = parameters.state.createXml())
        copyXmlToBinary(*xml, destData);
}

void SimpleGainAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        parameters.state = juce::ValueTree::fromXml(*xml);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleGainAudioProcessor();
}
