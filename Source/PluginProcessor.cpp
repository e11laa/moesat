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
    , parameters(*this, nullptr, juce::Identifier("tutorial"), // 追加
        {// 追加
            std::make_unique<juce::AudioParameterFloat>(
                "gain",  // 追加
                "Gain",  // 追加
                juce::NormalisableRange<float>(-100.0f, 10.0f), // 追加
                0.0f), // 追加
            std::make_unique<juce::AudioParameterChoice>(
                "panrule", "Pan Rule",
                juce::StringArray("linear", "balanced", "sin3dB", "sin4p5dB",
                                    "sin6dB", "squareRoot3dB",
                                    "squareRoot4p5dB"),
                1),
            std::make_unique<juce::AudioParameterFloat>(
                "panangle",  // 追加
                "Pan Angle",  // 追加
                juce::NormalisableRange<float>(-100.0f, 100.0f),// 追加
                0.0f),// 追加
            std::make_unique<juce::AudioParameterFloat>(
                "limit",
                "Limit",
                juce::NormalisableRange<float>(-100.0f, 0.0f), // 追加
                0.0f),
        })

{
    gain = parameters.getRawParameterValue("gain"); // 追加
    panAngle = parameters.getRawParameterValue("panangle"); // 追加
    panRule = parameters.getRawParameterValue("panrule");
    limit = parameters.getRawParameterValue("limit");// 追加
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
    spec.maximumBlockSize = samplesPerBlock; // 追加
    spec.numChannels = 2; // 追加
    spec.sampleRate = sampleRate; // 追加

    gainDSP.prepare(spec); // 追加
    pannerDSP.prepare(spec); // 追加
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

    gainDSP.setGainDecibels(*gain); // 追記
    pannerDSP.setRule(static_cast<juce::dsp::PannerRule>((int)*panRule)); // 追記
    pannerDSP.setPan(*panAngle / 100); // 追記

    juce::dsp::AudioBlock<float> audioBlock(buffer); // 追記
    juce::dsp::ProcessContextReplacing<float> context(audioBlock); // 追記

    gainDSP.process(context); // 追記
    pannerDSP.process(context); // 追記
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
    auto state = parameters.copyState(); // 追加
    std::unique_ptr<juce::XmlElement> xml(state.createXml()); // 追加
    copyXmlToBinary(*xml, destData); // 追加
}

void GainPantutorialAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes)); // 追加

    if (xmlState.get() != nullptr) // 追加
        if (xmlState->hasTagName(parameters.state.getType())) // 追加
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState)); // 追加
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainPantutorialAudioProcessor();
}
