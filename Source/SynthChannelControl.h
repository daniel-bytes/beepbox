#ifndef __SYNTHCHANNELCONTROL_H__
#define __SYNTHCHANNELCONTROL_H__

#include "../JuceLibraryCode/JuceHeader.h"

class SynthChannelControl
	: public Component,
	  public juce::Slider::Listener
{

private:
	int channel;
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