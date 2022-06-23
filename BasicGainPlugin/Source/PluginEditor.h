/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BasicGainPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BasicGainPluginAudioProcessorEditor (BasicGainPluginAudioProcessor&);
    ~BasicGainPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

	/* Look here */
	// Lets make a slider juce utilises Object Oriented Programming, meaning we have to make
	// an object and then modify it as a starting point

	// Make the Slider
	juce::Slider Gain;

	// Used to manage link between Volume slider and the DSP.
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>mVolAttachment;

	/* Have a look at how it's implemented in the PluginEditor.CPP*/


    BasicGainPluginAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicGainPluginAudioProcessorEditor)
};
