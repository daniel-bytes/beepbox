#ifndef __SYNTHCHANNEL_H__
#define __SYNTHCHANNEL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSource.h"

class ParameterBus;
class DiodeLadderFilter;

class SynthChannel
	: public ParameterSource
{
public:
	SynthChannel(ParameterBus *bus, int channelNumber);
	virtual ~SynthChannel(void);

public:
	virtual void onParameterUpdated(Parameter *parameter);
	void prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels);
	void releaseResources(void);
	void processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels);
	void trigger(double velocity);

private:
	void initializeIncrement(void);
	void incrementPhase(void);
	double getWaveformFromPhase(void);
	
	void initializeEnvelopeIncrement(void);
	void incrementEnvelopePhase(void);
	double getEnvelopeFromPhase(void);

private:
	enum class EnvelopeState { Off, Triggered };

private:
	ParameterBus *bus;
	int channelNumber;
	double phase;
	double increment;
	double envelopephase;
	double envelopeincrement;
	EnvelopeState envelopestate;
	int pitch;
	double gain;
	double sampleRate;
	double waveform;
	double decay;
	double velocity;
	double channelNormalize;
	OwnedArray<DiodeLadderFilter> filters;
};

#endif //__SYNTHCHANNEL_H__