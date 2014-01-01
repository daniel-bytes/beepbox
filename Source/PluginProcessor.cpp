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

#define STEP_SEQUENCER_DEFAULT_NUM_STEPS 8

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
	triggerNotificationQueue();
	monome->drawGrid();
}

//==============================================================================
void BeepBoxAudioProcessor::configureParameters(void)
{
	for (int i = 0; i < NUM_CHANNELS; i++) {
		configureChannelParameters(i);
	}

	configureIntParameter(ParameterID::Global_ActiveChannel, "Active Channel", 0, 0, NUM_CHANNELS, false);
	configureIntParameter(ParameterID::Global_SequencerStepCount, "Steps", 8, 4, 32, false);
	configureIntParameter(ParameterID::Global_SequencerPosition, "Position", 0, 0, 32, false);
	configureIntParameter(ParameterID::Global_SequencerStepResolution, "Clock Speed", 16, 1, 64, true);
}

void BeepBoxAudioProcessor::configureChannelParameters(int channel)
{
	String pitch = "Pitch ";
	String gain = "Gain ";
	String wave = "Waveform ";
	String attack = "Attack ";
	String decay = "Decay ";
	String data = "Sequencer Data ";

	pitch << (channel + 1);
	gain << (channel + 1);
	wave << (channel + 1);
	attack << (channel + 1);
	decay << (channel + 1);
	data << (channel + 1);

	configureIntParameter(GetChannelParameterID(ParameterID::Channel_Pitch, channel), pitch, 0, -60, 60, true);
	configureFloatParameter(GetChannelParameterID(ParameterID::Channel_Gain, channel), gain, .75f, 0.0f, 1.0f, true);
	configureFloatParameter(GetChannelParameterID(ParameterID::Channel_Waveform, channel), wave, 0, 0.0f, 1.0f, true);
	//configureFloatParameter(GetChannelParameterID(ParameterID::Channel_Attack, channel), attack, .75f, 0.0f, 1.0f, true);
	configureFloatParameter(GetChannelParameterID(ParameterID::Channel_Decay, channel), decay, .75f, 0.0f, 1.0f, true);
	configureReferenceCountedObjectParameter(GetChannelParameterID(ParameterID::Channel_SequencerData, channel), data, new SequencerData(STEP_SEQUENCER_DEFAULT_NUM_STEPS));
}

void BeepBoxAudioProcessor::forEachParameterSource(std::function<void(ParameterSource*)> callback)
{
	callback(synthChannels);
	callback(monome);

	auto editor = getActiveEditor();

	if (editor != nullptr) {
		auto editorSource = dynamic_cast<ParameterSource*>(editor);
		jassert(editorSource != nullptr);
		callback(editorSource);
	}
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
	
	synthChannels->onClockStep(posInfo.isPlaying, posInfo.ppqPosition);

	synthChannels->processBlock(buffer, getNumInputChannels(), getNumOutputChannels());

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool BeepBoxAudioProcessor::hasEditor() const
{
    return true;
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
