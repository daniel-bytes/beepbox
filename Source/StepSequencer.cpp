#include "StepSequencer.h"
#include "ParameterBus.h"

StepSequencer::StepSequencer(ParameterBus *bus, int channelNumber)
	: bus(bus),
	  channelNumber(channelNumber),
	  noteResolution(16),
	  currentStep(0)
{
}

StepSequencer::~StepSequencer(void)
{
}

SequencerStep StepSequencer::onClockStep(double ppq)
{
	SequencerStep result = {0};
	ParameterID dataid = GetChannelParameterID(ParameterID::Channel1_SequencerData, channelNumber);

	auto data = (SequencerData*)bus->getParameterValue(dataid).getObject();
	int resolution = bus->getParameterValue(ParameterID::StepSequencerResolution);

	int pulse = (int)floor(ppq * 24.0);
	int pulsesPerStep = (int)(96.0 / resolution);

	int clockPos = (pulse / pulsesPerStep) % data->getNumSteps();
	
	if (currentStep != clockPos) {
		currentStep = clockPos;

		result = data->getValue(currentStep);

		if ((int)bus->getParameterValue(ParameterID::StepSequencerPosition) != currentStep) {
			bus->updateParameter(this, ParameterID::StepSequencerPosition, currentStep);
		}
	}

	return result;
}

void StepSequencer::onParameterUpdated(Parameter *parameter)
{
	if (parameter->getParameterID() == ParameterID::StepSequencerResolution) {
		noteResolution = parameter->getValue();
	}
}