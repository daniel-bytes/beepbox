#include "StepSequencerControl.h"
#include "ParameterBus.h"
#include "SequencerData.h"

StepSequencerControl::StepSequencerControl(ParameterBus *bus)
	: bus(bus), lastX(-1), lastY(-1), drawMode(DrawMode::None)
{
}

StepSequencerControl::~StepSequencerControl(void)
{
}

void StepSequencerControl::paint(Graphics &g)
{
	const float stepPadding = 5.0f;
	const float cornerSize = 3.0f;

	int currentStep = bus->getParameterValue(ParameterID::Global_SequencerPosition);
	int stepCount = bus->getParameterValue(ParameterID::Global_SequencerStepCount);
	Rectangle<int> sequencerBounds(this->getWidth() - 2, this->getHeight() - 2);
	float fWidth = (float)sequencerBounds.getWidth();
	float fHeight = (float)sequencerBounds.getHeight();
	float stepWidth = fWidth / (float)stepCount;
	float channelHeight = fHeight / (float)NUM_CHANNELS;

	g.fillAll(Colours::white);
	g.drawRect(sequencerBounds, 2);

	// Draw current playback position
	g.setColour(Colours::red);
	float x = (float)currentStep * stepWidth;
	Rectangle<float> currentStepBounds(x, 0.f, stepWidth, fHeight);
	g.fillRect(x, 0.f, stepWidth, fHeight);

	// Draw sequencer
	for (int chan = 0; chan < NUM_CHANNELS; chan++) {
		SequencerData* data = bus->getChannelParameterObject<SequencerData>(ParameterID::Channel_SequencerData, chan);
		
		// draw channel dividers
		float y = (float)chan * channelHeight;
		g.setColour(Colours::grey);
		g.drawLine(0, y, fWidth, y);

		for (int step = 0; step < data->getNumSteps(); step++) {
			float x = (float)step * stepWidth;
		
			// draw step dividers
			g.setColour(Colours::grey);
			g.drawLine(x, 0, x, fHeight);

			// fill step value if set
			auto currentValue = data->getValue(step);

			if (currentValue.isSet) {
				g.setColour(Colours::green);
				g.fillRoundedRectangle(x + stepPadding, y + stepPadding, stepWidth - (stepPadding * 2), channelHeight - (stepPadding * 2), cornerSize);
			}
		}
	}
}


void StepSequencerControl::mouseDown(const MouseEvent &event)
{
	if (event.mods.isLeftButtonDown()) {
		lastX = -1;
		lastY = -1;

		onMouseEvent(event, false);
	}
}

void StepSequencerControl::mouseUp(const MouseEvent &event)
{
	if (event.mods.isLeftButtonDown()) {
		lastX = -1;
		lastY = -1;
		drawMode = DrawMode::None;
	}
}

void StepSequencerControl::mouseDrag(const MouseEvent &event)
{
	if (event.mods.isLeftButtonDown()) {
		onMouseEvent(event, true);
	}
}

void StepSequencerControl::onMouseEvent(const MouseEvent &event, bool isDrag)
{
	auto position = event.getPosition();

	// Get track number
	float ydistance = jlimit(0.f, .9999f, (float)position.getY() / (float)this->getHeight());
	int trackNumber = (int)floor(ydistance * (float)NUM_CHANNELS);

	jassert(trackNumber >= 0 && trackNumber < NUM_CHANNELS);

	// Get sequencer data for the track
	SequencerData* data = bus->getChannelParameterObject<SequencerData>(ParameterID::Channel_SequencerData, trackNumber);

	// Get the current step
	float xdistance = jlimit(0.f, .9999f, (float)position.getX() / (float)this->getWidth());
	int stepNumber = (int)floor(xdistance * (float)data->getNumSteps());

	jassert(stepNumber >= 0 && stepNumber < data->getNumSteps());

	// if we are dragging and we haven't hit a new part of the grid, return;
	if (isDrag && lastX == stepNumber && lastY == trackNumber) {
		return;
	}

	lastX = stepNumber;
	lastY = trackNumber;
	
	// Figure out what the current step value should be and update the ParameterBus.
	SequencerStep seqValue = data->getValue(stepNumber);

	switch(drawMode) {
	case DrawMode::None:
		seqValue.isSet = !seqValue.isSet;
		drawMode = seqValue.isSet ? DrawMode::Adding : DrawMode::Deleting;
		break;
	default:
		seqValue.isSet = drawMode == DrawMode::Adding;
		break;
	}
	
	seqValue.value = seqValue.isSet ? 1.f : 0.f;
	data->setValue(stepNumber, seqValue);
	bus->updateChannelParameterAndNotify(this, ParameterID::Channel_SequencerData, trackNumber, data);

	repaint();
}

void StepSequencerControl::onParameterUpdated(Parameter *parameter)
{
}