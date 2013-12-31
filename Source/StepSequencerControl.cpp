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
	int currentStep = bus->getParameterValue(ParameterID::StepSequencerPosition);

	for (int chan = 0; chan < NUM_CHANNELS; chan++) {
		auto id = GetChannelParameterID(ParameterID::Channel1_SequencerData, chan);
		auto data = static_cast<SequencerData*>(bus->getParameterValue(id).getObject());

		if (data == nullptr) {
			return;
		}

		Rectangle<int> sequencerBounds(this->getWidth(), this->getHeight());

		g.fillAll(Colours::white);

		g.drawRect(sequencerBounds, 2);

		float fWidth = (float)getWidth();
		float fHeight = (float)getHeight();
		float stepMult = fWidth / (float)data->getNumSteps();
		float rowMult = fHeight / (float)NUM_CHANNELS;

		// Set current playback position
		g.setColour(Colours::red);
		float x = (float)currentStep * stepMult;
		Rectangle<float> currentStepBounds(x, 0.f, stepMult, fHeight);
		g.fillRect(currentStepBounds);

		// draw rows and fill selected steps
		for (int step = 0; step < data->getNumSteps(); step++) {
			float x = (float)step * stepMult;
			auto currentValue = data->getValue(step);

			if (currentValue.isSet) {
				g.setColour(Colours::green);
			
				float y = (float)(chan * rowMult);
				g.fillRect(x + 1, y + 1, stepMult - 2, rowMult - 2);
			}

			g.setColour(Colours::grey);

			g.drawLine(x, 0, x, (float)getHeight());
		}

		// draw columns
		g.setColour(Colours::grey);

		for (int i = 0; i < NUM_CHANNELS; i++) {
			float y = (float)i * rowMult;
			g.drawLine(0, y, fWidth, y);
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

		onMouseEvent(event, false);
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

	float ydistance = jlimit(0.f, .9999f, (float)position.getY() / (float)this->getHeight());
	int trackNumber = (int)floor(ydistance * (float)NUM_CHANNELS);

	jassert(trackNumber >= 0 && trackNumber < NUM_CHANNELS);

	auto id = GetChannelParameterID(ParameterID::Channel1_SequencerData, trackNumber);
	auto data = static_cast<SequencerData*>(bus->getParameterValue(id).getObject());

	if (data == nullptr) {
		return;
	}

	// Get the current step based on mouse coordinates
	float xdistance = jlimit(0.f, .9999f, (float)position.getX() / (float)this->getWidth());
	int stepNumber = (int)floor(xdistance * (float)data->getNumSteps());

	// if we are dragging and we haven't hit a new part of the grid, return;
	if (isDrag && lastX == stepNumber && lastY == trackNumber) {
		return;
	}

	lastX = stepNumber;
	lastY = trackNumber;
	
	// create a new value to return
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
	bus->updateParameterAndNotify(this, id, data);

	repaint();
}

void StepSequencerControl::onParameterUpdated(Parameter *parameter)
{
}