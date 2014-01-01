#include "ChannelSelectorControl.h"
#include "Parameter.h"
#include "ParameterBus.h"

ChannelSelectorControl::ChannelSelectorControl(ParameterBus *bus)
	: bus(bus)
{
}

ChannelSelectorControl::~ChannelSelectorControl(void)
{
}

void ChannelSelectorControl::paint(Graphics &g)
{
	Rectangle<int> bounds(this->getWidth() - 2, this->getHeight() - 2);
	float fWidth = (float)bounds.getWidth();
	float fHeight = (float)bounds.getHeight();
	float channelHeight = fHeight / (float)NUM_CHANNELS;
	int activeChannel = bus->getParameterValue(ParameterID::Global_ActiveChannel);

	g.fillAll(Colours::white);
	
	g.setColour(Colours::black);
	g.drawRect(bounds, 2);

	for (int channel = 0; channel < NUM_CHANNELS; channel++) {
		float y = (float)channel * channelHeight;

		if (channel == activeChannel) {
			g.setColour(Colours::red);
			g.fillRect(0.f, y, fWidth, channelHeight);
		}

		g.setColour(Colours::grey);
		g.drawLine(0.f, y, fWidth, y);

		String text = "Channel ";
		text << (channel + 1);
		g.setColour(Colours::black);
		g.drawText(text, 0, (int)y, bounds.getWidth(), (int)channelHeight, Justification::centred, false);
	}
}

void ChannelSelectorControl::mouseDown(const MouseEvent &event)
{
	if (event.mods.isLeftButtonDown()) {
		auto position = event.getPosition();

		// Get track number
		float ydistance = jlimit(0.f, .9999f, (float)position.getY() / (float)this->getHeight());
		int channel = (int)floor(ydistance * (float)NUM_CHANNELS);

		bus->updateParameterAndNotify(this, ParameterID::Global_ActiveChannel, channel);
		repaint();
	}


}

void ChannelSelectorControl::onParameterUpdated(Parameter *parameter)
{
	repaint();
}