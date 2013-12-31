#ifndef __STEPSEQUENCERCONTROL_H__
#define __STEPSEQUENCERCONTROL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSource.h"

class ParameterBus;

class StepSequencerControl
	: public Component,
	  public ParameterSource
{
public:
	StepSequencerControl(ParameterBus *bus);
	virtual ~StepSequencerControl(void);

public:
	void paint(Graphics &g);
	void mouseDown(const MouseEvent &event);
	void mouseDrag(const MouseEvent &event);
	void mouseUp(const MouseEvent &event);
	void onParameterUpdated(Parameter *parameter);

protected:
	virtual void onMouseEvent(const MouseEvent &event, bool isDrag);


private:
	int lastX, lastY;
	ParameterBus *bus;

	enum class DrawMode
	{
		None,
		Adding,
		Deleting
	} drawMode;
};

#endif //__STEPSEQUENCERCONTROL_H__
