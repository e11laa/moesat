/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainPantutorialAudioProcessorEditor::GainPantutorialAudioProcessorEditor(
    GainPantutorialAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) // valueTreeStateを追加
    : AudioProcessorEditor(&p), audioProcessor(p), valueTreeState(vts) // valueTreeStateを追加
{
    gainSliderAttachment.reset(new SliderAttachment(valueTreeState, "gain", gainSlider));
    // ↓スライダーの種類を縦方向のスライダーにする
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    // ↓テキストボックスの位置をスライダーの下にして、高さと幅を調整する
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, width * 2 / 3, PannerAria.getHeight() / 6);
    addAndMakeVisible(gainSlider);


    gainSliderAttachment.reset(new SliderAttachment(valueTreeState, "gain", gainSlider));
    addAndMakeVisible(gainSlider);

    panAngleSliderAttachment.reset(new SliderAttachment(valueTreeState, "limit", panAngleSlider));
    // ↓スライダーのスタイルを回転ノブ型にして、操作方向は垂直にする
    panAngleSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    // ↓テキストボックスはノブの↓に表示し高さと幅を設定する
    panAngleSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, width * 2 / 3, PannerAria.getHeight() / 6);
    addAndMakeVisible(panAngleSlider);
    panRuleBox.addItemList(
        juce::StringArray("linear", "balanced", "sin3dB", "sin4.5dB", "sin6dB",
            "sqrt3dB", "sqrt4.5dB"),
        1); // 追加
    panRuleBoxAttachment.reset(new ComboBoxAttachment(valueTreeState, "panrule", panRuleBox)); // 追加
    addAndMakeVisible(panRuleBox); // 追加
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(width, height);
}

GainPantutorialAudioProcessorEditor::~GainPantutorialAudioProcessorEditor()
{
}

//==============================================================================
void GainPantutorialAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colour::Colour(46, 52, 64));
    g.fillRect(GainAria.toFloat());
    // ↓追記
    g.fillRect(PannerRuleAria.toFloat());

}

void GainPantutorialAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds(GainAria.reduced(5));
    panAngleSlider.setBounds(PannerAria.reduced(5));
    panRuleBox.setBounds(PannerRuleAria.reduced(width / 6, 4));
}