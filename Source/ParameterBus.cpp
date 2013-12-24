#include "ParameterBus.h"
#include "ParameterSource.h"

ParameterBus::ParameterBus(void)
{
}

ParameterBus::~ParameterBus(void)
{
	HashMap<ParameterID, Parameter*, ParameterIDHash>::Iterator it(parameters);

	while(it.next()) {
		delete it.getValue();
	}
}

void ParameterBus::updateParameter(ParameterSource *source, Parameter *parameter)
{
	auto parameterSources = getParameterSources();

	for (auto parameterSource : parameterSources) {
		if (parameterSource != nullptr && parameterSource != source) {
			parameterSource->onParameterUpdated(parameter);
		}
	}
}

void ParameterBus::configureStringParameter(ParameterID id, String name, String defaultValue)
{
	jassert(!parameters.contains(id));

	auto parameter = new StringParameter(id, name, defaultValue);
	parameters.set(id, parameter);
}

void ParameterBus::configureArrayParameter(ParameterID id, String name, Array<var> defaultValue)
{
	jassert(!parameters.contains(id));

	auto parameter = new ArrayParameter(id, name, defaultValue);
	parameters.set(id, parameter);
}

void ParameterBus::configureIntParameter(ParameterID id, String name, int defaultValue, int minValue, int maxValue, bool isAutomationParameter)
{
	jassert(!parameters.contains(id));

	auto parameter = new IntParameter(id, name, defaultValue, minValue, maxValue);
	parameters.set(id, parameter);

	if (isAutomationParameter) {
		automationParameterIDs.add(id);
	}
}

void ParameterBus::configureFloatParameter(ParameterID id, String name, float defaultValue, float minValue, float maxValue, bool isAutomationParameter)
{
	jassert(!parameters.contains(id));

	auto parameter = new FloatParameter(id, name, defaultValue, minValue, maxValue);
	parameters.set(id, parameter);

	if (isAutomationParameter) {
		automationParameterIDs.add(id);
	}
}

Parameter* ParameterBus::getParameter(ParameterID id) const
{
	return parameters[id];
}

var ParameterBus::getParameterValue(ParameterID id) const
{
	jassert(parameters.contains(id));

	return parameters[id]->getValue();
}

int ParameterBus::getAutomationParameterCount(void) const
{
	return automationParameterIDs.size();
}

float ParameterBus::getAutomationParameterValue(int index) const
{
	jassert(index < automationParameterIDs.size());

	ParameterID id = automationParameterIDs[index];
	return getParameter(id)->getNormalizedValue();
}

String ParameterBus::getAutomationParameterText(int index) const
{
	jassert(index < automationParameterIDs.size());

	ParameterID id = automationParameterIDs[index];
	return getParameter(id)->getDisplayValue();
}

String ParameterBus::getAutomationParameterName(int index) const
{
	jassert(index < automationParameterIDs.size());

	ParameterID id = automationParameterIDs[index];
	return getParameter(id)->getName();
}

void ParameterBus::setAutomationParameterValue(int index, float value)
{
	jassert(index < automationParameterIDs.size());

	ParameterID id = automationParameterIDs[index];
	Parameter *parameter = getParameter(id);

	parameter->setNormalizedValue(value);
}