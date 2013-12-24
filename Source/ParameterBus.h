#ifndef __PARAMETERBUS_H__
#define __PARAMETERBUS_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameter.h"

class ParameterSource;

class ParameterBus
{
public:
	ParameterBus(void);
	virtual ~ParameterBus(void);

public:
	virtual void configureParameters(void) = 0;
	virtual Array<ParameterSource*> getParameterSources(void) = 0;
	virtual void updateParameter(ParameterSource *source, Parameter *parameter);

	void configureStringParameter(ParameterID id, String name, String defaultValue);
	void configureArrayParameter(ParameterID id, String name, Array<var> defaultValue);
	void configureIntParameter(ParameterID id, String name, int defaultValue, int minValue, int maxValue, bool isAutomationParameter);
	void configureFloatParameter(ParameterID id, String name, float defaultValue, float minValue, float maxValue, bool isAutomationParameter);

	Parameter* getParameter(ParameterID id) const;
	var getParameterValue(ParameterID id) const;
	
	int getAutomationParameterCount(void) const;
	float getAutomationParameterValue(int index) const;
	String getAutomationParameterText(int index) const;
	String getAutomationParameterName(int index) const;
	void setAutomationParameterValue(int index, float value);

private:
	Array<ParameterID> automationParameterIDs;
	HashMap<ParameterID, Parameter*, ParameterIDHash> parameters;
};

#endif //__PARAMETERBUS_H__