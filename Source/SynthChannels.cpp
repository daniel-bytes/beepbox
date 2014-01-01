#include "SynthChannels.h"
#include "SynthChannel.h"
#include "ParameterBus.h"
#include "SequencerData.h"

SynthChannels::SynthChannels(ParameterBus *bus)
	: noteResolution(16),
	  currentStep(-1),
	  currentPPQ(-1.f),
	  bus(bus)
{
	jassert(bus != nullptr);

	for (int i = 0; i < NUM_CHANNELS; i++) {
		auto channel = new SynthChannel(bus, i);
		channels.add(channel);
	}
}

SynthChannels::~SynthChannels(void)
{
	channels.clear(true);
}

void SynthChannels::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	for (auto channel : channels) {
		channel->prepareToPlay(sampleRate, samplesPerBlock);
	}
}

void SynthChannels::releaseResources(void)
{
	for (auto channel : channels) {
		channel->releaseResources();
	}
}

void SynthChannels::onParameterUpdated(Parameter *parameter)
{
	int channelIndex = GetChannelIndex(parameter->getParameterID());

	if (channelIndex != INVALID_CHANNEL_INDEX) {
		jassert(channelIndex < channels.size());
		channels[channelIndex]->onParameterUpdated(parameter);
	}
}

void SynthChannels::processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels)
{
	for (auto channel : channels) {
		channel->processBlock(buffer, numInputChannels, numOutputChannels);
	}
}

void SynthChannels::onClockStep(bool isPlaying, double ppq)
{
	if (currentPPQ == ppq) {
		return;
	}

	int resolution = bus->getParameterValue(ParameterID::StepSequencerResolution);
	int numSteps = bus->getParameterValue(ParameterID::StepSequencerStepCount);

	int pulse = (int)floor(ppq * 24.0);
	int pulsesPerStep = (int)(96.0 / resolution);

	int clockPos = (pulse / pulsesPerStep) % numSteps;

	if (currentStep != clockPos) {
		currentStep = clockPos;

		bus->updateParameterAndNotify(this, ParameterID::StepSequencerPosition, currentStep);

		if (isPlaying) {
			for (int channel = 0; channel < channels.size(); channel++) {
				SequencerData *data = bus->getChannelParameterObject<SequencerData>(ParameterID::Channel1_SequencerData, channel);
				SequencerStep value = data->getValue(currentStep);

				if (value.isSet) {
					channels[channel]->trigger(value.value);
				}
			}
		}
	}

	currentPPQ = ppq;
}