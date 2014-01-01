#ifndef __CHANNELSELECTORCONTROL_H__
#define __CHANNELSELECTORCONTROL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSource.h"

class ParameterBus;

class ChannelSelectorControl
	: public Component,
	  public ParameterSource
{
public:
	ChannelSelectorControl(ParameterBus *bus);
	virtual ~ChannelSelectorControl(void);

public:
	virtual void paint(Graphics &g);
	virtual void mouseDown(const MouseEvent &event);
	virtual void onParameterUpdated(Parameter *parameter);

private:
	ParameterBus *bus;
};

#endif //__CHANNELSELECTORCONTROL_H__