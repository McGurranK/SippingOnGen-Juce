/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicGainPluginAudioProcessor::BasicGainPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	:m_CurrentBufferSize(0), AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)

#endif
{

		// use a default samplerate and vector size here, reset it later
	m_C74PluginState = (CommonState*)gen_export::create(44100, 64);
	gen_export::reset(m_C74PluginState);

	m_InputBuffers = new t_sample *[gen_export::num_inputs()];
	m_OutputBuffers = new t_sample *[gen_export::num_outputs()];

	for (int i = 0; i < gen_export::num_inputs(); i++) {
		m_InputBuffers[i] = NULL;
	}
	for (int i = 0; i < gen_export::num_outputs(); i++) {
		m_OutputBuffers[i] = NULL;
	}
	for (int i = 0; i < gen_export::num_params(); ++i)
	{
		auto name = juce::String(gen_export::getparametername(m_C74PluginState, i));
		apvts.addParameterListener(name, this);
	}
	
}


BasicGainPluginAudioProcessor::~BasicGainPluginAudioProcessor()
{
	delete m_InputBuffers;
	delete m_OutputBuffers;
	gen_export::destroy(m_C74PluginState);
}

//==============================================================================
const juce::String BasicGainPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicGainPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicGainPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicGainPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicGainPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicGainPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicGainPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicGainPluginAudioProcessor::setCurrentProgram (int index)
{
	index;
}

const juce::String BasicGainPluginAudioProcessor::getProgramName (int index)
{
	index;
    return {};
}

void BasicGainPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
	index;
	newName;
}

//==============================================================================
void BasicGainPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	m_C74PluginState->sr = sampleRate;
	m_C74PluginState->vs = samplesPerBlock;

	assureBufferSize(samplesPerBlock);
}

void BasicGainPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicGainPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BasicGainPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	// Unreferenced Pamater
	midiMessages;

	for (int i = 0; i < gen_export::num_inputs(); i++)
	{
		if (i < getTotalNumInputChannels())
		{
			for (int j = 0; j < m_CurrentBufferSize; j++)
				m_InputBuffers[i][j] = buffer.getReadPointer(i)[j];
		}
		else
		{
			memset(m_InputBuffers[i], 0, m_CurrentBufferSize * sizeof(double));
		}
	}

	gen_export::perform(m_C74PluginState,
		m_InputBuffers,
		gen_export::num_inputs(),
		m_OutputBuffers,
		gen_export::num_outputs(),
		buffer.getNumSamples());

	for (int i = 0; i < getTotalNumOutputChannels(); i++)
	{
		if (i < gen_export::num_outputs())
		{
			for (int j = 0; j < buffer.getNumSamples(); j++)
				buffer.getWritePointer(i)[j] = m_OutputBuffers[i][j];
		}
		else
		{
			buffer.clear(i, 0, buffer.getNumSamples());
		}
	}
}
// Added
void BasicGainPluginAudioProcessor::assureBufferSize(long bufferSize)
{
	if (bufferSize > m_CurrentBufferSize)
	{
		for (int i = 0; i < gen_export::num_inputs(); i++)
		{
			if (m_InputBuffers[i])
				delete m_InputBuffers[i];

			m_InputBuffers[i] = new t_sample[bufferSize];
		}

		for (int i = 0; i < gen_export::num_outputs(); i++)
		{
			if (m_OutputBuffers[i])
				delete m_OutputBuffers[i];

			m_OutputBuffers[i] = new t_sample[bufferSize];
		}

		m_CurrentBufferSize = bufferSize;
	}
}
void BasicGainPluginAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
	auto index = apvts.getParameter(parameterID)->getParameterIndex();
	gen_export::setparameter(m_C74PluginState, index, newValue, nullptr);
}

juce::AudioProcessorValueTreeState::ParameterLayout BasicGainPluginAudioProcessor::createParameterLayout()
{
	m_C74PluginState = (CommonState*)gen_export::create(44100, 64);
	gen_export::reset(m_C74PluginState);

	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	for (int i = 0; i < gen_export::num_params(); ++i)
	{
		auto name = juce::String(gen_export::getparametername(m_C74PluginState,i));
		auto min = gen_export::getparametermin(m_C74PluginState, i);
		auto max = gen_export::getparametermax(m_C74PluginState, i);
		auto defaultValue = m_C74PluginState->params[i].defaultvalue;

		layout.add(std::make_unique<juce::AudioParameterFloat>(name, name,
			juce::NormalisableRange<float>(min, max, 0.01f, 1.f),
			defaultValue,
			juce::String(),
			juce::AudioProcessorParameter::genericParameter,
			nullptr,
			nullptr));
	}

	return layout;
}

//==============================================================================
bool BasicGainPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicGainPluginAudioProcessor::createEditor()
{
	return new BasicGainPluginAudioProcessorEditor(*this);

	// Generic Audio Processor Editor
	//return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
// XLM Save State

void BasicGainPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{

	destData.setSize(gen_export::getstatesize(m_C74PluginState), false);
	gen_export::getstate(m_C74PluginState, (char*)destData.getData());

}

void BasicGainPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{

	gen_export::setstate(m_C74PluginState, (const char *)data);
	sizeInBytes;

}

//==============================================================================
// This creates new instances of the plugin..

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{

	return new BasicGainPluginAudioProcessor();
}

