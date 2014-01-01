#include "ParameterID.h"

bool IsChannelParameterID(ParameterID id)
{
	return (int)id < GLOBAL_PARAMETER_OFFSET;
}

int GetChannelIndex(ParameterID id)
{
	if (!IsChannelParameterID(id)) {
		return INVALID_CHANNEL_INDEX;
	}

	return (int)id / CHANNEL_PARAMETER_OFFSET;
}

ParameterID GetBaseParameterID(ParameterID id, int channel)
{
	jassert(channel < NUM_CHANNELS);

	int baseid = (int)id % CHANNEL_PARAMETER_OFFSET;

	jassert(baseid < CHANNEL_PARAMETER_OFFSET);

	return (ParameterID)baseid;
}

ParameterID GetChannelParameterID(ParameterID baseid, int channel)
{
	jassert((int)baseid < CHANNEL_PARAMETER_OFFSET);
	jassert(channel < NUM_CHANNELS);

	int offset = (CHANNEL_PARAMETER_OFFSET * channel);
	int id = (int)baseid + offset;

	return (ParameterID)id;
}