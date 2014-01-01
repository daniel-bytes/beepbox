#include "SynthChannel.h"
#include "ParameterBus.h"
#include "Utilities.h"

#define BASE_PITCH 60

SynthChannel::SynthChannel(ParameterBus *bus, int channelNumber)
	: channelNumber(channelNumber),
	  sampleRate(0),
	  phase(0),
	  pitch(0),
	  increment(0),
	  gain(1.0),
	  waveform(0),
	  decay(0),
	  envelopephase(0),
	  envelopeincrement(0),
	  envelopestate(EnvelopeState::Off),
	  velocity(0),
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
	initializeEnvelopeIncrement();
}

void SynthChannel::releaseResources(void)
{
}

void SynthChannel::processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels)
{
	for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
		for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
			float *outputdata = buffer.getSampleData(channel);
			double sampledata = getWaveformFromPhase() * getEnvelopeFromPhase() * gain * velocity;

			*(outputdata + sample) += (float)sampledata;
		}

		incrementPhase();
		incrementEnvelopePhase();
	}
}

void SynthChannel::trigger(double velocity)
{
	envelopestate = EnvelopeState::Triggered;
	envelopephase = 0;
	this->velocity = velocity;
}

void SynthChannel::initializeIncrement(void)
{
	double frequency = pitchToFrequency(BASE_PITCH + pitch);
	increment = (1.0 / sampleRate) * frequency;
}

void SynthChannel::initializeEnvelopeIncrement(void)
{
	double scaledDecay = scale(decay, 20.0, 1.0);
	envelopeincrement = (1.0 / sampleRate) * scaledDecay;
}

void SynthChannel::incrementPhase(void)
{
	if ((phase += increment) >= 1.0) {
		phase -= 1.0;
	}
}

void SynthChannel::incrementEnvelopePhase(void)
{
	if (envelopestate == EnvelopeState::Triggered) {
		if ((envelopephase += envelopeincrement) > 1.0) {
			envelopestate = EnvelopeState::Off;
			envelopephase = 0;
		}
	}
}

double SynthChannel::getWaveformFromPhase(void)
{
	// just a basic sawtooth for now
	double saw = (phase * 2.0) - 1.0;
	double square = (saw > 0) ? 1.0 : -1.0;

	return (saw * waveform) + (square * (1.0 - waveform));
}

double SynthChannel::getEnvelopeFromPhase(void)
{
	if (envelopestate == EnvelopeState::Off) {
		return 0;
	}

	double exponentialPhase = jlimit(0.0, 1.0, envelopephase * envelopephase);
	return 1.0 - exponentialPhase;
}

void SynthChannel::onParameterUpdated(Parameter *parameter)
{
	ParameterID baseid = GetBaseParameterID(parameter->getParameterID(), channelNumber);

	switch(baseid) {
		case ParameterID::Channel_Gain:
			this->gain = parameter->getValue();
			break;
		case ParameterID::Channel_Pitch:
			this->pitch = parameter->getValue();
			initializeIncrement();
			break;
		case ParameterID::Channel_Waveform:
			this->waveform = parameter->getValue();
			break;
		case ParameterID::Channel_Decay:
			this->decay = parameter->getValue();
			initializeEnvelopeIncrement();
			break;
	}
}