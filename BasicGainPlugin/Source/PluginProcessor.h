/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
// Location for gen_export header file including
#include "gen_export/gen_export.h"

//==============================================================================
/**
*/
class BasicGainPluginAudioProcessor  : public juce::AudioProcessor,
	juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    BasicGainPluginAudioProcessor();
    ~BasicGainPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	using APVTS = juce::AudioProcessorValueTreeState;
	APVTS::ParameterLayout createParameterLayout();
	APVTS apvts{ *this, nullptr, "Parameters", createParameterLayout() };

	void parameterChanged(const juce::String& parameterID, float newValue) override;

protected:

	void assureBufferSize(long bufferSize);


private:
    //==============================================================================
	CommonState *m_C74PluginState;
	long m_CurrentBufferSize;
	t_sample** m_InputBuffers;
	t_sample** m_OutputBuffers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicGainPluginAudioProcessor)
};
