#ifndef __MONOMECONTROLSURFACE_H__
#define __MONOMECONTROLSURFACE_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSource.h"
#include "SerialOscController.h"

class ParameterBus;

class MonomeControlSurface
	: public ParameterSource,
	  public SerialOscController::Listener
{
public:
	MonomeControlSurface(ParameterBus *bus);
	virtual ~MonomeControlSurface(void);

public:
	virtual void buttonPressMessageReceived(int x, int y, bool state);
	virtual void onParameterUpdated(Parameter *parameter);

	void drawGrid(void);

private:
	ParameterBus *bus;
	ScopedPointer<SerialOscController> serialosc;
};

#endif //__MONOMECONTROLSURFACE_H__