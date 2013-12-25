#include "SequencerData.h"

SequencerData::SequencerData(int numSteps)
{
	setNumSteps(numSteps);
}

SequencerData::~SequencerData(void)
{
}

void SequencerData::setNumSteps(int value)
{
	int oldSize = steps.size();

	steps.resize(value);

	for (int i = oldSize; i < steps.size(); i++) {
		SequencerStep step = {0};
		steps.set(i, step);
	}
}

SequencerStep SequencerData::getValue(int step)
{
	return steps[step];
}

void SequencerData::setValue(int step, SequencerStep value)
{
	jassert(step < steps.size());

	steps.set(step, value);
}
