/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "SurgeFXProcessor.h"
#include "SurgeFXEditor.h"

//==============================================================================
SurgefxAudioProcessor::SurgefxAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withInput  ("SideChain", AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                       )
{
    addParameter(mix = new AudioParameterFloat("mix", "Mix", 0.f, 1.f, 0.2f));
    addParameter(feedback = new AudioParameterFloat("feedback", "Feedback", 0.f, 1.f, 0.7f));
}

SurgefxAudioProcessor::~SurgefxAudioProcessor()
{
}

//==============================================================================
const String SurgefxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SurgefxAudioProcessor::acceptsMidi() const
{
    return false;
}

bool SurgefxAudioProcessor::producesMidi() const
{
    return false;
}

bool SurgefxAudioProcessor::isMidiEffect() const
{
    return false;
}

double SurgefxAudioProcessor::getTailLengthSeconds() const
{
    return 2.0;
}

int SurgefxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SurgefxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SurgefxAudioProcessor::setCurrentProgram (int index)
{
}

const String SurgefxAudioProcessor::getProgramName (int index)
{
    return "Default";
}

void SurgefxAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SurgefxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    std::cout << "prepareToPlay" << std::endl;
    delayRingL.resize(20000);
    delayRingR.resize(20000);
    for( int i=0; i<20000; ++i )
    {
        delayRingL[i] = 0.;
        delayRingR[i] = 0.;
    }
    ringIndex = 0;
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SurgefxAudioProcessor::releaseResources()
{
    std::cout << "releaseResources" << std::endl;

    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SurgefxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    return true;
}
#endif

void SurgefxAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* inData = buffer.getReadPointer(channel);
        auto* channelData = buffer.getWritePointer (channel);

        for( auto j=0; j<buffer.getNumSamples(); ++j )
        {
            // I know this is stupid code
            float m = *mix;
            float f = *feedback;
            if( channel == 0 )
            {
                channelData[j] = inData[j] * (1.f-m)  + delayRingR[ringIndex] * m;
                delayRingR[ringIndex] = inData[j] * (1-f) + delayRingR[ringIndex] * f;;
            }
            else
            {
                channelData[j] = inData[j] * (1.f-m)  + delayRingL[ringIndex] * m;
                delayRingL[ringIndex] = inData[j] * (1-f) + delayRingL[ringIndex] * f;;
            }
            ringIndex ++;
            if( ringIndex >= 20000 ) ringIndex = 0;
        }

    }
}

//==============================================================================
bool SurgefxAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SurgefxAudioProcessor::createEditor()
{
    return nullptr; // new SurgefxAudioProcessorEditor (*this);
}

//==============================================================================
void SurgefxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SurgefxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SurgefxAudioProcessor();
}
