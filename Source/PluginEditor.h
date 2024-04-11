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
class GainPantutorialAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    GainPantutorialAudioProcessorEditor(GainPantutorialAudioProcessor&,
        juce::AudioProcessorValueTreeState& vts); // juce::AudioÅ`vtsÇí«â¡
    ~GainPantutorialAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    int width = 150;
    int height = 550;

    // Å´èCê≥
    juce::Rectangle<int> GainAria{ width, height * 4 / 5 - 30 };
    // Å´èCê≥
    juce::Rectangle<int> PannerAria{ 0, GainAria.getBottom(), width, height / 5 };
    // Å´í«ãL
    juce::Rectangle<int> PannerRuleAria{ 0, PannerAria.getBottom(), width, height - GainAria.getHeight() - PannerAria.getHeight() };
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainPantutorialAudioProcessor& audioProcessor;

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment; // í«â¡
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment; // í«â¡

    juce::AudioProcessorValueTreeState& valueTreeState; // í«â¡
    juce::Slider gainSlider; // í«â¡
    std::unique_ptr<SliderAttachment> gainSliderAttachment; // í«â¡
    juce::Slider panAngleSlider; // í«â¡
    std::unique_ptr<SliderAttachment> panAngleSliderAttachment; // í«â¡
    juce::ComboBox panRuleBox; // í«â¡
    std::unique_ptr<ComboBoxAttachment> panRuleBoxAttachment; // í«â¡


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainPantutorialAudioProcessorEditor)
};