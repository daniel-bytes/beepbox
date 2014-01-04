#ifndef __PARAMETERID_H__
#define __PARAMETERID_H__

#include "../JuceLibraryCode/JuceHeader.h"

#define NUM_CHANNELS 6
#define INVALID_CHANNEL_INDEX -1
#define CHANNEL_PARAMETER_OFFSET 100
#define GLOBAL_PARAMETER_OFFSET 1000

/*
 * Each channel based set of parameters starts at a multiple of 100.
 * So, channel 1 starts at 0, 2 at 100, etc.
 * This allows a channel to be determined from a ParameterID by dividing by 100.
 *
 * Non-channel based (global) parameters start at 1000.
 */
enum class ParameterID
{
	// Channel Data
	Channel_Pitch = 1,
	Channel_Gain = 2,
	Channel_Waveform = 3,
	Channel_Attack = 4,
	Channel_Decay = 5,
	Channel_SequencerData = 6,
	Channel_Cutoff = 7,
	Channel_Resonance = 8,

	// Global
	Global_ActiveChannel = 1001,
	Global_SequencerStepCount = 1002,
	Global_SequencerPosition = 1003,
	Global_SequencerStepResolution = 1004
};

struct ParameterIDHash
{
	 int generateHash (const ParameterID key, const int upperLimit) const noexcept
	 {
		 int iKey = (int)key;
		 return std::abs (iKey) % upperLimit; 
	 }
};

bool IsChannelParameterID(ParameterID id);

int GetChannelIndex(ParameterID id);

ParameterID GetBaseParameterID(ParameterID id, int channel);

ParameterID GetChannelParameterID(ParameterID baseid, int channel);

#endif //__PARAMETERID_H__