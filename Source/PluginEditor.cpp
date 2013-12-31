#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParameterBus.h"
#include "StepSequencerControl.h"

//==============================================================================
BeepBoxAudioProcessorEditor::BeepBoxAudioProcessorEditor (BeepBoxAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
	bus = (ParameterBus*)ownerFilter;

    // This is where our plugin's editor size is set.
    setSize (800, 600);

	stepSequencerControl = new StepSequencerControl(bus);
	addAndMakeVisible(stepSequencerControl);
	stepSequencerControl->setBounds(50, 50, 600, 300);
}

BeepBoxAudioProcessorEditor::~BeepBoxAudioProcessorEditor()
{
	stepSequencerControl = nullptr;
}

//==============================================================================
void BeepBoxAudioProcessorEditor::onParameterUpdated(Parameter *parameter)
{
	if (parameter->getParameterID() == ParameterID::StepSequencerPosition) {
		stepSequencerControl->repaint();
	}
}

//==============================================================================
void BeepBoxAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!",
                      0, 0, getWidth(), getHeight(),
                      Justification::centred, 1);
}
