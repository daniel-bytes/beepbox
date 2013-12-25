/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthChannels.h"
#include "SequencerData.h"
#include "MonomeControlSurface.h"

//==============================================================================
BeepBoxAudioProcessor::BeepBoxAudioProcessor()
{
	configureParameters();

	synthChannels = new SynthChannels(this);
	initializeSource(synthChannels);

	monome = new MonomeControlSurface(this);
	initializeSource(monome);

	startTimer(100);
}

BeepBoxAudioProcessor::~BeepBoxAudioProcessor()
{
	stopTimer();
	synthChannels = nullptr;
	monome = nullptr;
}

void BeepBoxAudioProcessor::timerCallback(void)
{
	monome->drawGrid();
}

//==============================================================================
void BeepBoxAudioProcessor::configureParameters(void)
{
	configureIntParameter(ParameterID::Channel1_Pitch, "Pitch 1", 0, -60, 60, true);
	configureFloatParameter(ParameterID::Channel1_Gain, "Gain 1", .75f, 0.0f, 1.0f, true);
	configureFloatParameter(ParameterID::Channel1_Waveform, "Wave 1", 0, 0.0f, 1.0f, true);
	configureFloatParameter(ParameterID::Channel1_Decay, "Decay 1", .75f, 0.0f, 1.0f, true);
	configureReferenceCountedObjectParameter(ParameterID::Channel1_SequencerData, "Sequencer Data 1", new SequencerData(8));

	configureIntParameter(ParameterID::ActiveChannel, "Active Channel", 0, 0, NUM_CHANNELS, false);
	configureIntParameter(ParameterID::StepSequencerStepCount, "Steps", 8, 4, 32, false);
	configureIntParameter(ParameterID::StepSequencerPosition, "Position", 0, 0, 32, false);
	configureIntParameter(ParameterID::StepSequencerResolution, "Clock Speed", 8, 1, 64, true);

	// TEST
	auto d = (SequencerData*)getParameterValue(ParameterID::Channel1_SequencerData).getObject();
	SequencerStep step = {1.0, true};
	d->setValue(0, step);
	d->setValue(4, step);
}

Array<ParameterSource*> BeepBoxAudioProcessor::getParameterSources(void)
{
	Array<ParameterSource*> sources;

	// add all sources here
	sources.add(synthChannels);
	sources.add(monome);

	auto editor = getActiveEditor();

	if (editor != nullptr) {
		sources.add((ParameterSource*)editor);
	}

	return sources;
}

//==============================================================================
const String BeepBoxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int BeepBoxAudioProcessor::getNumParameters()
{
	return getAutomationParameterCount();
}

float BeepBoxAudioProcessor::getParameter (int index)
{
	return getAutomationParameterValue(index);
}

void BeepBoxAudioProcessor::setParameter (int index, float newValue)
{
	setAutomationParameterValue(index, newValue);
}

const String BeepBoxAudioProcessor::getParameterName (int index)
{
	return getAutomationParameterName(index);
}

const String BeepBoxAudioProcessor::getParameterText (int index)
{
	return getAutomationParameterText(index);
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
void BeepBoxAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	synthChannels->prepareToPlay(sampleRate, samplesPerBlock);
}

void BeepBoxAudioProcessor::releaseResources()
{
	synthChannels->releaseResources();
}

void BeepBoxAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    AudioPlayHead::CurrentPositionInfo posInfo;
	this->getPlayHead()->getCurrentPosition(posInfo);

	if (posInfo.isPlaying) {
		synthChannels->onClockStep(posInfo.ppqPosition);
	}

	synthChannels->processBlock(buffer, getNumInputChannels(), getNumOutputChannels());

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
    return false;
}

AudioProcessorEditor* BeepBoxAudioProcessor::createEditor()
{
    auto editor = new BeepBoxAudioProcessorEditor(this);
	
	initializeSource(editor);

	return editor;
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
