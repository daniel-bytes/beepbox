#ifndef __SYNTHCHANNEL_H__
#define __SYNTHCHANNEL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSource.h"

class ParameterBus;

class SynthChannel
	: public ParameterSource
{
public:
	SynthChannel(ParameterBus *bus, int channelNumber);
	virtual ~SynthChannel(void);

public:
	virtual void onParameterUpdated(Parameter *parameter);
	void prepareToPlay(double sampleRate, int samplesPerBlock);
	void releaseResources(void);
	void processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels);

private:
	void initializeIncrement(void);
	void incrementPhase(void);
	double getWaveformFromPhase(void);

private:
	ParameterBus *bus;
	int channelNumber;
	double phase;
	double increment;
	int pitch;
	double gain;
	double sampleRate;
	double waveform;
};

#endif //__SYNTHCHANNEL_H__