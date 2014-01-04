#include "SynthChannelControl.h"
#include "ParameterBus.h"

SynthChannelControl::SynthChannelControl(ParameterBus *bus)
	: bus(bus),
	  pitchLabel("PitchLabel", "Pitch"),
	  gainLabel("gainLabel", "Gain"),
	  waveformLabel("waveformLabel", "Waveform"),
	  decayLabel("decayLabel", "Decay"),
	  cutoffLabel("cutoffLabel", "Cutoff"),
	  resLabel("resLabel", "Resonance")
{
	int channel = 0;

	configureSlider(ParameterID::Channel_Gain, gainSlider, gainLabel, channel++);
	configureSlider(ParameterID::Channel_Pitch, pitchSlider, pitchLabel, channel++);
	configureSlider(ParameterID::Channel_Waveform, waveformSlider, waveformLabel, channel++);
	configureSlider(ParameterID::Channel_Decay, decaySlider, decayLabel, channel++);
	configureSlider(ParameterID::Channel_Cutoff, cutoffSlider, cutoffLabel, channel++);
	configureSlider(ParameterID::Channel_Resonance, resSlider, resLabel, channel++);

	updateSliderValues();
}

SynthChannelControl::~SynthChannelControl(void)
{
}

void SynthChannelControl::updateSliderValues(void)
{
	setSliderValue(ParameterID::Channel_Gain, gainSlider);
	setSliderValue(ParameterID::Channel_Pitch, pitchSlider);
	setSliderValue(ParameterID::Channel_Waveform, waveformSlider);
	setSliderValue(ParameterID::Channel_Decay, decaySlider);
	setSliderValue(ParameterID::Channel_Cutoff, cutoffSlider);
	setSliderValue(ParameterID::Channel_Resonance, resSlider);
}

void SynthChannelControl::onParameterUpdated(Parameter *parameter)
{
	switch (parameter->getParameterID()) {
	case ParameterID::Global_ActiveChannel:
		updateSliderValues();
		break;
	}
}

void SynthChannelControl::sliderValueChanged (Slider* slider)
{
	int channel = bus->getParameterValue(ParameterID::Global_ActiveChannel);

	if (slider == &gainSlider) {
		bus->updateChannelParameterAndNotify(this, ParameterID::Channel_Gain, channel, slider->getValue());
	}
	else if (slider == &pitchSlider) {
		bus->updateChannelParameterAndNotify(this, ParameterID::Channel_Pitch, channel, slider->getValue());
	}
	else if (slider == &waveformSlider) {
		bus->updateChannelParameterAndNotify(this, ParameterID::Channel_Waveform, channel, slider->getValue());
	}
	else if (slider == &decaySlider) {
		bus->updateChannelParameterAndNotify(this, ParameterID::Channel_Decay, channel, slider->getValue());
	}
	else if (slider == &cutoffSlider) {
		bus->updateChannelParameterAndNotify(this, ParameterID::Channel_Cutoff, channel, slider->getValue());
	}
	else if (slider == &resSlider) {
		bus->updateChannelParameterAndNotify(this, ParameterID::Channel_Resonance, channel, slider->getValue());
	}
}

void SynthChannelControl::configureSlider(ParameterID id, Slider &slider, Label &label, int position)
{
	const int xOffset = 120 * position;
	const Font labelFont(14.f, Font::FontStyleFlags::bold);
	const Font valueFont(14.f);
	
	int channel = bus->getParameterValue(ParameterID::Global_ActiveChannel);
	Parameter *parameter = bus->getChannelParameter(id, channel);
	double defaultValue = 0;
	double minValue = 0;
	double maxValue = 0;

	auto floatParameter = dynamic_cast<FloatParameter*>(parameter);

	if (floatParameter != nullptr) {
		defaultValue = floatParameter->getDefaultValue();
		minValue = floatParameter->getMinValue();
		maxValue = floatParameter->getMaxValue();
	}
	else {
		auto intParameter = dynamic_cast<IntParameter*>(parameter);

		if (intParameter != nullptr) {
			defaultValue = intParameter->getDefaultValue();
			minValue = intParameter->getMinValue();
			maxValue = intParameter->getMaxValue();
		}
	}

	addAndMakeVisible(&slider);
	slider.setDoubleClickReturnValue(true, defaultValue);
	slider.setBounds(xOffset, 50, 100, 100);
	slider.setRange(minValue, maxValue);
	slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
	slider.addListener(this);
    label.attachToComponent(&slider, false);
    label.setFont(labelFont);
}

void SynthChannelControl::setSliderValue(ParameterID id, Slider &slider)
{
	int channel = bus->getParameterValue(ParameterID::Global_ActiveChannel);
	auto value = bus->getChannelParameterValue(id, channel);
	slider.setValue((double)value);
}