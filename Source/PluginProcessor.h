#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterBus.h"

class SynthChannels;
class MonomeControlSurface;

//==============================================================================
/**
*/
class BeepBoxAudioProcessor  
	: public AudioProcessor,
	  public ParameterBus,
	  public Timer
{
public:
    //==============================================================================
    BeepBoxAudioProcessor();
    ~BeepBoxAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

	//==============================================================================
	virtual void configureParameters(void);
	virtual void configureChannelParameters(int channel);
	virtual void forEachParameterSource(std::function<void(ParameterSource*)> callback);

	//==============================================================================
	void timerCallback();

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;


    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

private:
	ScopedPointer<SynthChannels> synthChannels;
	ScopedPointer<MonomeControlSurface> monome;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BeepBoxAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
