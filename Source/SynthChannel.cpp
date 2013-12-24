#include "SynthChannel.h"
#include "ParameterBus.h"

SynthChannel::SynthChannel(ParameterBus *bus, int channelNumber)
	: channelNumber(channelNumber),
	  sampleRate(0),
	  phase(0),
	  pitch(0),
	  increment(0),
	  gain(1.0),
	  waveform(0),
	  bus(bus)
{
	jassert(bus != nullptr);
}

SynthChannel::~SynthChannel(void)
{
}

void SynthChannel::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	this->sampleRate = sampleRate;
	initializeIncrement();
}

void SynthChannel::releaseResources(void)
{
}

void SynthChannel::processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels)
{
	for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
		for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
			float *outputdata = buffer.getSampleData(channel);
			double sampledata = getWaveformFromPhase() * gain;

			*(outputdata + sample) += (float)sampledata;
		}

		incrementPhase();
	}
}

void SynthChannel::initializeIncrement(void)
{
	// TODO
}

void SynthChannel::incrementPhase(void)
{
	if ((phase += increment) >= 1.0) {
		phase -= 1.0;
	}
}

double SynthChannel::getWaveformFromPhase(void)
{
	// just a basic sawtooth for now
	return (phase * 2.0) - 1.0;
}

void SynthChannel::onParameterUpdated(Parameter *parameter)
{
	int localid = (int)parameter->getParameterID() % CHANNEL_PARAMETER_OFFSET;

	switch((ParameterID)localid) {
		case ParameterID::Channel1_Gain:
			this->gain = parameter->getValue();
			break;
		case ParameterID::Channel1_Pitch:
			this->pitch = parameter->getValue();
			initializeIncrement();
			break;
		case ParameterID::Channel1_Waveform:
			this->waveform = parameter->getValue();
			break;
	}
}