#include "SynthChannels.h"
#include "SynthChannel.h"
#include "ParameterBus.h"

SynthChannels::SynthChannels(ParameterBus *bus)
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
	int channelIndex = (int)parameter->getParameterID() / CHANNEL_PARAMETER_OFFSET;

	jassert(channelIndex < channels.size());
	channels[channelIndex]->onParameterUpdated(parameter);
}

void SynthChannels::processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels)
{
	for (auto channel : channels) {
		channel->processBlock(buffer, numInputChannels, numOutputChannels);
	}
}