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
        juce::AudioProcessorValueTreeState& vts); // juce::Audio�`vts��ǉ�
    ~GainPantutorialAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    int width = 150;
    int height = 550;

    // ���C��
    juce::Rectangle<int> GainAria{ width, height * 4 / 5 - 30 };
    // ���C��
    juce::Rectangle<int> PannerAria{ 0, GainAria.getBottom(), width, height / 5 };
    // ���ǋL
    juce::Rectangle<int> PannerRuleAria{ 0, PannerAria.getBottom(), width, height - GainAria.getHeight() - PannerAria.getHeight() };
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainPantutorialAudioProcessor& audioProcessor;

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment; // �ǉ�
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment; // �ǉ�

    juce::AudioProcessorValueTreeState& valueTreeState; // �ǉ�
    juce::Slider gainSlider; // �ǉ�
    std::unique_ptr<SliderAttachment> gainSliderAttachment; // �ǉ�
    juce::Slider panAngleSlider; // �ǉ�
    std::unique_ptr<SliderAttachment> panAngleSliderAttachment; // �ǉ�
    juce::ComboBox panRuleBox; // �ǉ�
    std::unique_ptr<ComboBoxAttachment> panRuleBoxAttachment; // �ǉ�


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainPantutorialAudioProcessorEditor)
};