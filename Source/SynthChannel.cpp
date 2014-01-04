#include "SynthChannel.h"
#include "ParameterBus.h"
#include "Utilities.h"
#include "DiodeLadderFilter.h"

#define BASE_PITCH 60
#define FB_HPF_CUTOFF .01

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
	  channelNormalize(1.0 / (double)NUM_CHANNELS),
	  bus(bus)
{
	jassert(bus != nullptr);
}

SynthChannel::~SynthChannel(void)
{
	filters.clear();
}

void SynthChannel::prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels)
{
	filters.clear(true);

	for (int i = 0; i < numOutputChannels; i++) {
		auto filter = new DiodeLadderFilter();
		filter->set_feedback_hpf_cutoff(FB_HPF_CUTOFF);
		filter->set_fc((double)bus->getChannelParameterValue(ParameterID::Channel_Cutoff, this->channelNumber));
		filter->set_q((double)bus->getChannelParameterValue(ParameterID::Channel_Resonance, this->channelNumber));

		filters.add(filter);
	}

	this->sampleRate = sampleRate;
	initializeIncrement();
	initializeEnvelopeIncrement();
}

void SynthChannel::releaseResources(void)
{
	filters.clear(true);
}

void SynthChannel::processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels)
{
	for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
		for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
			auto filter = filters[channel];
			float *outputdata = buffer.getSampleData(channel);
			
			// calculate waveform
			double sampledata = getWaveformFromPhase();

			// apply filter
			sampledata = filter->tick(sampledata);
			
			// apply envelope and gain
			sampledata *= (getEnvelopeFromPhase() * gain * velocity * channelNormalize);
			
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
	double scaledDecay = scale(decay * decay, 30.0, 1.0);
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
	double pulse = phase > jlimit(.1, .9, scale(waveform * 2.0, .5, .95)) ? 1.0 : -1.0;

	if (waveform <= .5) {
		return pulse;
	}
	else {
		double randomValue = phase > scale(random.nextDouble(), .1, .9) ? 1.0 : -1.0;
		return ((1.0 - waveform) * pulse) + (randomValue * pulse); 
	}
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
		case ParameterID::Channel_Cutoff:
			{
				for (int i = 0; i < filters.size(); i++) {
					filters[i]->set_fc((double)parameter->getValue());
				}
			}
			break;
		case ParameterID::Channel_Resonance:
			{
				for (int i = 0; i < filters.size(); i++) {
					filters[i]->set_q((double)parameter->getValue());
				}
			}
			break;
	}
}