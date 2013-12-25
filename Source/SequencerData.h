#ifndef __SEQUENCERDATA_H__
#define __SEQUENCERDATA_H__

#include "../JuceLibraryCode/JuceHeader.h"

struct SequencerStep
{
	float value;
	bool isSet;
};

class SequencerData
	: public ReferenceCountedObject 
{
public:
	SequencerData(int numSteps);
	virtual ~SequencerData(void);

	int getNumSteps(void) const { return steps.size(); }
	void setNumSteps(int value);

	SequencerStep getValue(int step);
	void setValue(int step, SequencerStep value);

public:
	typedef ReferenceCountedObjectPtr<SequencerData> Ptr;

private:
	Array<SequencerStep> steps;
};

#endif //__SEQUENCERDATA_H__