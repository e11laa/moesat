/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainPantutorialAudioProcessor::GainPantutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    , parameters(*this, nullptr, juce::Identifier("tutorial"), // �ǉ�
        {// �ǉ�
            std::make_unique<juce::AudioParameterFloat>(
                "gain",  // �ǉ�
                "Gain",  // �ǉ�
                juce::NormalisableRange<float>(-100.0f, 10.0f), // �ǉ�
                0.0f), // �ǉ�
            std::make_unique<juce::AudioParameterChoice>(
                "panrule", "Pan Rule",
                juce::StringArray("linear", "balanced", "sin3dB", "sin4p5dB",
                                    "sin6dB", "squareRoot3dB",
                                    "squareRoot4p5dB"),
                1),
            std::make_unique<juce::AudioParameterFloat>(
                "panangle",  // �ǉ�
                "Pan Angle",  // �ǉ�
                juce::NormalisableRange<float>(-100.0f, 100.0f),// �ǉ�
                0.0f),// �ǉ�
            std::make_unique<juce::AudioParameterFloat>(
                "limit",
                "Limit",
                juce::NormalisableRange<float>(-100.0f, 0.0f), // �ǉ�
                0.0f),
        })

{
    gain = parameters.getRawParameterValue("gain"); // �ǉ�
    panAngle = parameters.getRawParameterValue("panangle"); // �ǉ�
    panRule = parameters.getRawParameterValue("panrule");
    limit = parameters.getRawParameterValue("limit");// �ǉ�
}

GainPantutorialAudioProcessor::~GainPantutorialAudioProcessor()
{
}

//==============================================================================
const juce::String GainPantutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainPantutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainPantutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainPantutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainPantutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainPantutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GainPantutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainPantutorialAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GainPantutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void GainPantutorialAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GainPantutorialAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    spec.maximumBlockSize = samplesPerBlock; // �ǉ�
    spec.numChannels = 2; // �ǉ�
    spec.sampleRate = sampleRate; // �ǉ�

    gainDSP.prepare(spec); // �ǉ�
    pannerDSP.prepare(spec); // �ǉ�
}

void GainPantutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainPantutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GainPantutorialAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    gainDSP.setGainDecibels(*gain); // �ǋL
    pannerDSP.setRule(static_cast<juce::dsp::PannerRule>((int)*panRule)); // �ǋL
    pannerDSP.setPan(*panAngle / 100); // �ǋL

    juce::dsp::AudioBlock<float> audioBlock(buffer); // �ǋL
    juce::dsp::ProcessContextReplacing<float> context(audioBlock); // �ǋL

    gainDSP.process(context); // �ǋL
    pannerDSP.process(context); // �ǋL
}


//==============================================================================
bool GainPantutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GainPantutorialAudioProcessor::createEditor()
{
    return new GainPantutorialAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void GainPantutorialAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState(); // �ǉ�
    std::unique_ptr<juce::XmlElement> xml(state.createXml()); // �ǉ�
    copyXmlToBinary(*xml, destData); // �ǉ�
}

void GainPantutorialAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes)); // �ǉ�

    if (xmlState.get() != nullptr) // �ǉ�
        if (xmlState->hasTagName(parameters.state.getType())) // �ǉ�
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState)); // �ǉ�
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainPantutorialAudioProcessor();
}
