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
    //setResizable(true, true);
    //setResizeLimits(300, 500, 600, 1000);

    //const float ratio = 0.6f;
    //getConstrainer()->setFixedAspectRatio(ratio);

    setSize(300, 400);

    juce::Font monoBoldFont(juce::Font::getDefaultMonospacedFontName(), 15.0f, juce::Font::bold);

    invLeftPhaseButton.setButtonText(u8"\u00D8 L");
    invRightPhaseButton.setButtonText(u8"\u00D8 R");
    monoButton.setButtonText("Mono");
    bassMonoButton.setButtonText("Bass Mono");

    bassPreviewButton.setButtonText(u8"\U0001F3A7");
    muteButton.setButtonText("Mute");
    dcButton.setButtonText("DC");

    widthLabel.setText("Width", juce::NotificationType::dontSendNotification);
    widthLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    widthLabel.setFont(monoBoldFont);
    widthLabel.setJustificationType(juce::Justification::centredBottom);

    widthSlider.setLookAndFeel(&lnf);
    widthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    widthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    widthSlider.setTextValueSuffix("%");

    gainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    gainLabel.setFont(monoBoldFont);
    gainLabel.setJustificationType(juce::Justification::centredBottom);

    gainSlider.setLookAndFeel(&lnf);
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainSlider.setTextValueSuffix("dB");

    balanceLabel.setText("Balance", juce::NotificationType::dontSendNotification);
    balanceLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    balanceLabel.setFont(monoBoldFont);
    balanceLabel.setJustificationType(juce::Justification::centredBottom);


    balanceSlider.setLookAndFeel(&lnf);
    balanceSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    balanceSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    balanceSlider.setTextValueSuffix("<balance>");

    bassCrossoverSlider.setLookAndFeel(&lnf);
    bassCrossoverSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    bassCrossoverSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    bassCrossoverSlider.setTextValueSuffix("Hz");


    inputLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    inputLabel.setText("Input", juce::NotificationType::dontSendNotification);
    inputLabel.setFont(monoBoldFont);
    inputLabel.setJustificationType(juce::Justification::centred);

    outputLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    outputLabel.setText("Output", juce::NotificationType::dontSendNotification);
    outputLabel.setFont(monoBoldFont);
    outputLabel.setJustificationType(juce::Justification::centred);

    modeComboBox.setLookAndFeel(&lnf);
    modeComboBox.addItem("Stereo", 1);
    modeComboBox.addItem("Left", 2);
    modeComboBox.addItem("Right", 3);
    modeComboBox.addItem("Swap", 4);
    modeComboBox.setSelectedId(1);


    invLeftPhaseButton.setLookAndFeel(&lnf);
    invRightPhaseButton.setLookAndFeel(&lnf);
    monoButton.setLookAndFeel(&lnf);
    bassMonoButton.setLookAndFeel(&lnf);
    bassPreviewButton.setLookAndFeel(&lnf);
    muteButton.setLookAndFeel(&lnf);
    dcButton.setLookAndFeel(&lnf);

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
    addAndMakeVisible(widthLabel);
    addAndMakeVisible(balanceLabel);
    addAndMakeVisible(gainLabel);

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    widthSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "Width", widthSlider);
    gainSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "Gain", gainSlider);
    balanceSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "Balance", balanceSlider);
    bassCrossoverSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "BassMonoCrossover", bassCrossoverSlider);
    modeComboBoxAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.apvts, "Mode", modeComboBox);

    invLeftPhaseButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "InvertPhaseLeft", invLeftPhaseButton);
    invRightPhaseButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "InvertPhaseRight", invRightPhaseButton);
    monoButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "Mono", monoButton);
    bassMonoButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "BassMono", bassMonoButton);
    bassPreviewButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "BassMonoPreview", bassPreviewButton);
    muteButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "Mute", muteButton);
    dcButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "DC", dcButton);
}

UtilityAudioProcessorEditor::~UtilityAudioProcessorEditor()
{
    widthSlider.setLookAndFeel(nullptr);
    gainSlider.setLookAndFeel(nullptr);
    balanceSlider.setLookAndFeel(nullptr);
    bassCrossoverSlider.setLookAndFeel(nullptr);
    modeComboBox.setLookAndFeel(nullptr);

    invLeftPhaseButton.setLookAndFeel(nullptr);
    invRightPhaseButton.setLookAndFeel(nullptr);
    monoButton.setLookAndFeel(nullptr);
    bassMonoButton.setLookAndFeel(nullptr);
    bassPreviewButton.setLookAndFeel(nullptr);
    muteButton.setLookAndFeel(nullptr);
    dcButton.setLookAndFeel(nullptr);
}

//==============================================================================
void UtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    g.fillAll(juce::Colours::lightgrey);
}

void UtilityAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    using namespace juce;

    auto area = getLocalBounds();
    auto left = area.withTrimmedRight(area.getWidth() / 2);
    auto right = area.withTrimmedLeft(area.getWidth() / 2);

    int itemHeight = 40;
    int knobHeight = itemHeight * 2.6;
    int itemMargin = 6;

    inputLabel.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));

    auto buttonArea = left.removeFromTop(itemHeight).reduced(itemMargin);
    auto buttonWidth = buttonArea.getWidth() / 2;
    invLeftPhaseButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    invRightPhaseButton.setBounds(buttonArea);

    modeComboBox.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));

    widthLabel.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));

    widthSlider.setBounds(left.removeFromTop(knobHeight).reduced(itemMargin));
    monoButton.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));
    bassMonoButton.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));

    auto bxAndPArea = left.removeFromTop(itemHeight).reduced(itemMargin);
    auto bxAndPWidth = bxAndPArea.getWidth() / 2;

    bassCrossoverSlider.setBounds(bxAndPArea.removeFromLeft(bxAndPWidth));
    bassPreviewButton.setBounds(bxAndPArea);

    outputLabel.setBounds(right.removeFromTop(itemHeight).reduced(itemMargin));

    gainLabel.setBounds(right.removeFromTop(itemHeight).reduced(itemMargin));
    gainSlider.setBounds(right.removeFromTop(knobHeight).reduced(itemMargin));

    right.removeFromTop(itemMargin * 2.7);

    balanceLabel.setBounds(right.removeFromTop(itemHeight).reduced(itemMargin));
    balanceSlider.setBounds(right.removeFromTop(knobHeight).reduced(itemMargin));

    auto muteDcArea = right.removeFromTop(itemHeight).reduced(itemMargin);
    auto muteWidth = muteDcArea.getWidth() / 2;
    muteButton.setBounds(muteDcArea.removeFromLeft(muteWidth));
    dcButton.setBounds(muteDcArea);
}
