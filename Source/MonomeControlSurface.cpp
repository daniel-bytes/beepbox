#include "MonomeControlSurface.h"
#include "ParameterBus.h"
#include "SequencerData.h"

#define OSC_PREFIX "beepbox"
#define LISTEN_PORT (SERIALOSC_PORT + 1)
#define MAX_ROWS 8
#define MAX_COLUMNS 8

MonomeControlSurface::MonomeControlSurface(ParameterBus *bus)
	: bus(bus)
{
	serialosc = new SerialOscController(OSC_PREFIX, LISTEN_PORT);
	serialosc->start(this);
}

MonomeControlSurface::~MonomeControlSurface(void)
{
	serialosc->stop();
	serialosc = nullptr;
}

void MonomeControlSurface::onParameterUpdated(Parameter *parameter)
{
	
}

void MonomeControlSurface::buttonPressMessageReceived(int x, int y, bool state)
{
}

void MonomeControlSurface::drawGrid(void)
{
	Array<int> rowdata;
	rowdata.resize(MAX_ROWS);

	for (int i = 0; i < NUM_CHANNELS; i++) {
		ParameterID dataid = GetChannelParameterID(ParameterID::Channel1_SequencerData, i);
		auto data = (SequencerData*)bus->getParameterValue(dataid).getObject();

		int bits = 0;
		for (int j = 0; j < data->getNumSteps() && j < MAX_COLUMNS; j++) {
			auto step = data->getValue(j);

			if (step.isSet) {
				bits |= (1 << j);
			}
		}
		rowdata.set(i, bits);
	}

	serialosc->sendDeviceLedMapCommand(0, 0, rowdata);
}