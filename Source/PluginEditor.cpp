/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UtilityAudioProcessorEditor::UtilityAudioProcessorEditor (UtilityAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    invLeftPhaseButton.setButtonText("Inv L");
    invRightPhaseButton.setButtonText("Inv R");
    monoButton.setButtonText("Mono");
    bassMonoButton.setButtonText("Bass Mono");
    bassPreviewButton.setButtonText("Preview");
    muteButton.setButtonText("Mute");
    dcButton.setButtonText("DC");

    widthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    bassCrossoverSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 20);

    inputLabel.setText("Input", juce::NotificationType::dontSendNotification);
    outputLabel.setText("Output", juce::NotificationType::dontSendNotification);

    modeComboBox.addItem("Stereo", 1);
    modeComboBox.addItem("Left", 2);
    modeComboBox.addItem("Right", 3);
    modeComboBox.addItem("Swap", 4);
    modeComboBox.setSelectedId(1);

    addAndMakeVisible(invLeftPhaseButton);
    addAndMakeVisible(invRightPhaseButton);
    addAndMakeVisible(monoButton);
    addAndMakeVisible(bassMonoButton);
    addAndMakeVisible(bassPreviewButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(dcButton);

    addAndMakeVisible(widthSlider);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(balanceSlider);
    addAndMakeVisible(bassCrossoverSlider);

    addAndMakeVisible(modeComboBox);

    addAndMakeVisible(inputLabel);
    addAndMakeVisible(outputLabel);

    setSize (300, 500);
}

UtilityAudioProcessorEditor::~UtilityAudioProcessorEditor()
{
}

//==============================================================================
void UtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void UtilityAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    using namespace juce;

    auto area = getLocalBounds();
    auto left = area.withTrimmedRight(area.getWidth() / 2);
    auto right = area.withTrimmedLeft(area.getWidth() / 2);

    int itemHeight = 30;
    int itemMargin = 4;
    inputLabel.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));

    auto buttonArea = left.removeFromTop(itemHeight).reduced(itemMargin);
    auto buttonWidth = buttonArea.getWidth() / 2;
    invLeftPhaseButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    invRightPhaseButton.setBounds(buttonArea);

    modeComboBox.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));
    widthSlider.setBounds(left.removeFromTop(itemHeight * 4).reduced(itemMargin));
    monoButton.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));
    bassMonoButton.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));

    auto bxAndPArea = left.removeFromTop(itemHeight * 2).reduced(itemMargin);
    auto bxAndPWidth = bxAndPArea.getWidth() / 2;
    bassCrossoverSlider.setBounds(bxAndPArea.removeFromLeft(buttonWidth));
    bassPreviewButton.setBounds(bxAndPArea);


    outputLabel.setBounds(right.removeFromTop(itemHeight).reduced(itemMargin));
}
