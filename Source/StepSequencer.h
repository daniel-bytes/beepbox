#ifndef __STEPSEQUENCER_H__
#define __STEPSEQUENCER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSource.h"
#include "SequencerData.h"

class ParameterBus;

class StepSequencer
	: public ParameterSource
{
public:
	StepSequencer(ParameterBus *bus, int channelNumber);
	virtual ~StepSequencer(void);

	SequencerStep onClockStep(double ppq);

	int getNoteResolution(void) const { return noteResolution; }
	void setNoteResolution(int value) { noteResolution = value; }

	virtual void onParameterUpdated(Parameter *parameter);

private:
	ParameterBus *bus;
	int channelNumber;
	int noteResolution;
	int currentStep;
};

#endif //__STEPSEQUENCER_H__