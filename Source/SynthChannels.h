#ifndef __SYNTHCHANNELS_H__
#define __SYNTHCHANNELS_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSource.h"

class SynthChannel;
class ParameterBus;

class SynthChannels
	: public ParameterSource
{
public:
	SynthChannels(ParameterBus *bus);
	~SynthChannels(void);

public:
	virtual void onParameterUpdated(Parameter *parameter);
	void prepareToPlay(double sampleRate, int samplesPerBlock);
	void releaseResources(void);
	void processBlock(AudioSampleBuffer& buffer, int numInputChannels, int numOutputChannels);

private:
	OwnedArray<SynthChannel> channels;
};

#endif //__SYNTHCHANNELS_H__