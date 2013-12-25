#include "SynthChannels.h"
#include "SynthChannel.h"
#include "StepSequencer.h"
#include "ParameterBus.h"

SynthChannels::SynthChannels(ParameterBus *bus)
{
	jassert(bus != nullptr);

	for (int i = 0; i < NUM_CHANNELS; i++) {
		auto channel = new SynthChannel(bus, i);
		channels.add(channel);

		auto stepSequencer = new StepSequencer(bus, i);
		sequencers.add(stepSequencer);
	}
}

SynthChannels::~SynthChannels(void)
{
	channels.clear(true);
	sequencers.clear(true);
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
	if ((int)parameter->getParameterID() < GLOBAL_PARAMETER_OFFSET) {
		int channelIndex = (int)parameter->getParameterID() / CHANNEL_PARAMETER_OFFSET;

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

void SynthChannels::onClockStep(double ppq)
{
	for (int i = 0; i < channels.size(); i++) {
		auto value = sequencers[i]->onClockStep(ppq);

		if (value.isSet) {
			channels[i]->trigger(value.value);
		}
	}
}