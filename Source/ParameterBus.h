#ifndef __PARAMETERBUS_H__
#define __PARAMETERBUS_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameter.h"
#include <queue>

class ParameterSource;

class ParameterBus
{
public:
	ParameterBus(void);
	virtual ~ParameterBus(void);

	Parameter* getParameter(ParameterID id) const;
	var getParameterValue(ParameterID id) const;
	void initializeSource(ParameterSource *source);

	virtual void triggerNotificationQueue(void);
	virtual void updateParameterAndNotify(ParameterSource *source, ParameterID id, var value);
	virtual void updateParameterAndQueueNotification(ParameterSource *source, ParameterID id, var value, int deferTicks = 0);

protected:
	virtual void configureParameters(void) = 0;
	virtual Array<ParameterSource*> getParameterSources(void) = 0;

	void configureStringParameter(ParameterID id, String name, String defaultValue);
	void configureArrayParameter(ParameterID id, String name, Array<var> defaultValue);
	void configureIntParameter(ParameterID id, String name, int defaultValue, int minValue, int maxValue, bool isAutomationParameter);
	void configureFloatParameter(ParameterID id, String name, float defaultValue, float minValue, float maxValue, bool isAutomationParameter);
	void configureReferenceCountedObjectParameter(ParameterID id, String name, ReferenceCountedObject *value);

	int getAutomationParameterCount(void) const;
	float getAutomationParameterValue(int index) const;
	String getAutomationParameterText(int index) const;
	String getAutomationParameterName(int index) const;
	void setAutomationParameterValue(int index, float value);

	void notifySources(ParameterSource *source, ParameterID id);

private:
	struct NotificationQueueItem
	{
		ParameterSource *source;
		ParameterID id;
		int tickCount;
	};

private:
	Array<ParameterID> automationParameterIDs;
	HashMap<ParameterID, Parameter*, ParameterIDHash> parameters;
	std::queue<NotificationQueueItem> notificationQueue;
};

#endif //__PARAMETERBUS_H__