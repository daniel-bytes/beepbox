#ifndef __PARAMETERID_H__
#define __PARAMETERID_H__

#define NUM_CHANNELS 1
#define CHANNEL_PARAMETER_OFFSET 100

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
	
	// Channel 2
	Channel2_Pitch = 101,
	Channel2_Gain = 102,
	Channel2_Waveform = 103,

	// Global
};

struct ParameterIDHash
{
	 int generateHash (const ParameterID key, const int upperLimit) const noexcept
	 {
		 int iKey = (int)key;
		 return std::abs (iKey) % upperLimit; 
	 }
};

#endif //__PARAMETERID_H__