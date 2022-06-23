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
class _1_GenToJuceAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _1_GenToJuceAudioProcessorEditor (_1_GenToJuceAudioProcessor&);
    ~_1_GenToJuceAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _1_GenToJuceAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_1_GenToJuceAudioProcessorEditor)
};
