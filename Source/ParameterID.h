#ifndef __PARAMETERID_H__
#define __PARAMETERID_H__

#include "../JuceLibraryCode/JuceHeader.h"

#define NUM_CHANNELS 6
#define CHANNEL_PARAMETER_OFFSET 100
#define GLOBAL_PARAMETER_OFFSET 1000

/*
 * Each channel based set of parameters starts at a multiple of 100.
 * So, channel 1 starts at 0, 2 at 100, etc.
 * This allows a channel to be determined from a ParameterID by dividing by 100.
 *
 * - Example:  int channelIndex = (int)parameter->getParameterID() / CHANNEL_PARAMETER_OFFSET;
 *
 * Non-channel based (global) parameters start at 1000.
 */
enum class ParameterID
{
	// Channel 1
	Channel1_Pitch = 1,
	Channel1_Gain = 2,
	Channel1_Waveform = 3,
	Channel1_Attack = 4,
	Channel1_Decay = 5,
	Channel1_SequencerData = 6,

	// Global
	ActiveChannel = 1001,
	StepSequencerStepCount = 1002,
	StepSequencerPosition = 1003,
	StepSequencerResolution = 1004
};

struct ParameterIDHash
{
	 int generateHash (const ParameterID key, const int upperLimit) const noexcept
	 {
		 int iKey = (int)key;
		 return std::abs (iKey) % upperLimit; 
	 }
};


ParameterID GetBaseParameterID(ParameterID id, int channel);

ParameterID GetChannelParameterID(ParameterID baseid, int channel);

#endif //__PARAMETERID_H__