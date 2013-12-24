#ifndef __PARAMETERID_H__
#define __PARAMETERID_H__

enum class ParameterID
{
	SynthChannel1_Pitch = 1,
	SynthChannel1_Gain = 2,
	SynthChannel1_Waveform = 3,
	
	SynthChannel2_Pitch = 101,
	SynthChannel2_Gain = 102,
	SynthChannel2_Waveform = 103,

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