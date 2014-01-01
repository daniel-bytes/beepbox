#ifndef __SYNTHCHANNELCONTROL_H__
#define __SYNTHCHANNELCONTROL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSource.h"

class ParameterBus;

class SynthChannelControl
	: public Component,
	  public juce::Slider::Listener,
	  public ParameterSource
{
public:
	SynthChannelControl(ParameterBus *bus);
	virtual ~SynthChannelControl(void);

public:
	virtual void onParameterUpdated(Parameter *parameter);
	virtual void sliderValueChanged (Slider* slider);
	void updateSliderValues(void);

private:
	void configureSlider(ParameterID id, Slider &slider, Label &label, int position);
	void setSliderValue(ParameterID id, Slider &slider);

private:
	ParameterBus *bus;

	Slider pitchSlider, 
		   gainSlider, 
		   waveformSlider,
		   decaySlider;

	Label pitchLabel,
		  gainLabel,
		  waveformLabel,
		  decayLabel;
};

#endif //__SYNTHCHANNELCONTROL_H__