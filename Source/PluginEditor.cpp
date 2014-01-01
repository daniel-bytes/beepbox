#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParameterBus.h"
#include "StepSequencerControl.h"
#include "SynthChannelControl.h"
#include "ChannelSelectorControl.h"

//==============================================================================
BeepBoxAudioProcessorEditor::BeepBoxAudioProcessorEditor (BeepBoxAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
	bus = (ParameterBus*)ownerFilter;

    // This is where our plugin's editor size is set.
    setSize (800, 600);

	synthChannelControl = new SynthChannelControl(bus);
	addAndMakeVisible(synthChannelControl);
	synthChannelControl->setBounds(25, 25, 750, 200);

	channelSelectorControl = new ChannelSelectorControl(bus);
	addAndMakeVisible(channelSelectorControl);
	channelSelectorControl->setBounds(25, 225, 100, 300);

	stepSequencerControl = new StepSequencerControl(bus);
	addAndMakeVisible(stepSequencerControl);
	stepSequencerControl->setBounds(125, 225, 650, 300);
}

BeepBoxAudioProcessorEditor::~BeepBoxAudioProcessorEditor()
{
	synthChannelControl = nullptr;
	stepSequencerControl = nullptr;
	channelSelectorControl = nullptr;
}

//==============================================================================
void BeepBoxAudioProcessorEditor::onParameterUpdated(Parameter *parameter)
{
	switch (parameter->getParameterID()) {
	case ParameterID::Global_SequencerPosition:
		stepSequencerControl->repaint();
		break;
	case ParameterID::Global_ActiveChannel:
		synthChannelControl->updateSliderValues();
		break;
	}
}

//==============================================================================
void BeepBoxAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
}
