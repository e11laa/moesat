/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainPantutorialAudioProcessorEditor::GainPantutorialAudioProcessorEditor(
    GainPantutorialAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) // valueTreeState��ǉ�
    : AudioProcessorEditor(&p), audioProcessor(p), valueTreeState(vts) // valueTreeState��ǉ�
{
    gainSliderAttachment.reset(new SliderAttachment(valueTreeState, "gain", gainSlider));
    // ���X���C�_�[�̎�ނ��c�����̃X���C�_�[�ɂ���
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    // ���e�L�X�g�{�b�N�X�̈ʒu���X���C�_�[�̉��ɂ��āA�����ƕ��𒲐�����
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, width * 2 / 3, PannerAria.getHeight() / 6);
    addAndMakeVisible(gainSlider);


    gainSliderAttachment.reset(new SliderAttachment(valueTreeState, "gain", gainSlider));
    addAndMakeVisible(gainSlider);

    panAngleSliderAttachment.reset(new SliderAttachment(valueTreeState, "limit", panAngleSlider));
    // ���X���C�_�[�̃X�^�C������]�m�u�^�ɂ��āA��������͐����ɂ���
    panAngleSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    // ���e�L�X�g�{�b�N�X�̓m�u�́��ɕ\���������ƕ���ݒ肷��
    panAngleSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, width * 2 / 3, PannerAria.getHeight() / 6);
    addAndMakeVisible(panAngleSlider);
    panRuleBox.addItemList(
        juce::StringArray("linear", "balanced", "sin3dB", "sin4.5dB", "sin6dB",
            "sqrt3dB", "sqrt4.5dB"),
        1); // �ǉ�
    panRuleBoxAttachment.reset(new ComboBoxAttachment(valueTreeState, "panrule", panRuleBox)); // �ǉ�
    addAndMakeVisible(panRuleBox); // �ǉ�
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
    // ���ǋL
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