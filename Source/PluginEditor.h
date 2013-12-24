/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ParameterSource.h"

class ParameterBus;

//==============================================================================
/**
*/
class BeepBoxAudioProcessorEditor  
	: public AudioProcessorEditor,
	  public ParameterSource
{
public:
    BeepBoxAudioProcessorEditor (BeepBoxAudioProcessor* ownerFilter);
    ~BeepBoxAudioProcessorEditor();

	//==============================================================================
	virtual void onParameterUpdated(Parameter *parameter);

    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);

private:
	ParameterBus *bus;
};


#endif  // PLUGINEDITOR_H_INCLUDED
