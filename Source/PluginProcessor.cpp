/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
BeepBoxAudioProcessor::BeepBoxAudioProcessor()
{
}

BeepBoxAudioProcessor::~BeepBoxAudioProcessor()
{
}

//==============================================================================
const String BeepBoxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int BeepBoxAudioProcessor::getNumParameters()
{
    return 0;
}

float BeepBoxAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void BeepBoxAudioProcessor::setParameter (int index, float newValue)
{
}

const String BeepBoxAudioProcessor::getParameterName (int index)
{
    return String::empty;
}

const String BeepBoxAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String BeepBoxAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String BeepBoxAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool BeepBoxAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool BeepBoxAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool BeepBoxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BeepBoxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BeepBoxAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double BeepBoxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BeepBoxAudioProcessor::getNumPrograms()
{
    return 0;
}

int BeepBoxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BeepBoxAudioProcessor::setCurrentProgram (int index)
{
}

const String BeepBoxAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void BeepBoxAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BeepBoxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BeepBoxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void BeepBoxAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
       // float* channelData = buffer.getSampleData (channel);

        // ..do something to the data...
    }

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool BeepBoxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BeepBoxAudioProcessor::createEditor()
{
    return new BeepBoxAudioProcessorEditor (this);
}

//==============================================================================
void BeepBoxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BeepBoxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BeepBoxAudioProcessor();
}
