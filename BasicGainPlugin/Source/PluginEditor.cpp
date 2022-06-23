/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicGainPluginAudioProcessorEditor::BasicGainPluginAudioProcessorEditor (BasicGainPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
	// Making a Slider (Look at the PluginEditor.h)
	
	addAndMakeVisible(Gain); // Makes it on the screen

	// What type of slider and does it have a text box.
	
	Gain.setSliderStyle(juce::Slider::SliderStyle::Rotary);  // Rotary slider
	Gain.setTextBoxStyle(juce::Slider::NoTextBox, false,0,0); // No textbox for the value

	// Linking to the apvts for controlling the instrument
	// The first value is the APVTs set up in the processor.
	// Second is the name of the Gen Gain param
	// Third is the name of the slider object
	mVolAttachment = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>
	(audioProcessor.apvts, "Gain", Gain);


	// Plugin window size (X,Y)
    setSize (400, 300);			
}

BasicGainPluginAudioProcessorEditor::~BasicGainPluginAudioProcessorEditor()
{
}

//==============================================================================
void BasicGainPluginAudioProcessorEditor::paint (juce::Graphics& g)
{


}

// Setup size of all the objects
void BasicGainPluginAudioProcessorEditor::resized()
{
	// Gain slider poistion on screen and size
	Gain.setBounds(100, 100, 100, 100);
}
