#ifndef __PARAMETERBUS_H__
#define __PARAMETERBUS_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Parameter.h"
#include <queue>
#include <functional>

class ParameterSource;

class ParameterBus
{
public:
	ParameterBus(void);
	virtual ~ParameterBus(void);

public:
	Parameter* getParameter(ParameterID id) const;
	Parameter* getChannelParameter(ParameterID baseid, int channel) const;

	var getParameterValue(ParameterID id) const;
	var getChannelParameterValue(ParameterID baseid, int channel) const;

	template<typename TObject> TObject* getParameterObject(ParameterID id);
	template<typename TObject> TObject* getChannelParameterObject(ParameterID baseid, int channel);

	void initializeSource(ParameterSource *source);
	virtual void triggerNotificationQueue(void);

	virtual void updateParameterAndNotify(ParameterSource *source, ParameterID id, var value);
	virtual void updateChannelParameterAndNotify(ParameterSource *source, ParameterID baseid, int channel, var value);
	
	virtual void updateParameterAndQueueNotification(ParameterSource *source, ParameterID id, var value, int deferTicks = 0);
	virtual void updateChannelParameterAndQueueNotification(ParameterSource *source, ParameterID baseid, int channel, var value, int deferTicks = 0);

protected:
	virtual void configureParameters(void) = 0;
	virtual void forEachParameterSource(std::function<void(ParameterSource*)> callback) = 0;

protected:
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

// Template definitions
template<typename TObject> 
TObject* ParameterBus::getParameterObject(ParameterID id)
{
	var value = getParameterValue(id);
	auto result = static_cast<TObject*>(value.getObject());
	jassert(result != nullptr);

	return result;
}

template<typename TObject> 
TObject* ParameterBus::getChannelParameterObject(ParameterID baseid, int channel)
{
	ParameterID id = GetChannelParameterID(baseid, channel);

	return getParameterObject<TObject>(id);
}

#endif //__PARAMETERBUS_H__