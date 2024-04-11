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
        juce::AudioProcessorValueTreeState& vts); // juce::Audio〜vtsを追加
    ~GainPantutorialAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    int width = 150;
    int height = 550;

    // ↓修正
    juce::Rectangle<int> GainAria{ width, height * 4 / 5 - 30 };
    // ↓修正
    juce::Rectangle<int> PannerAria{ 0, GainAria.getBottom(), width, height / 5 };
    // ↓追記
    juce::Rectangle<int> PannerRuleAria{ 0, PannerAria.getBottom(), width, height - GainAria.getHeight() - PannerAria.getHeight() };
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainPantutorialAudioProcessor& audioProcessor;

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment; // 追加
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment; // 追加

    juce::AudioProcessorValueTreeState& valueTreeState; // 追加
    juce::Slider gainSlider; // 追加
    std::unique_ptr<SliderAttachment> gainSliderAttachment; // 追加
    juce::Slider panAngleSlider; // 追加
    std::unique_ptr<SliderAttachment> panAngleSliderAttachment; // 追加
    juce::ComboBox panRuleBox; // 追加
    std::unique_ptr<ComboBoxAttachment> panRuleBoxAttachment; // 追加


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainPantutorialAudioProcessorEditor)
};