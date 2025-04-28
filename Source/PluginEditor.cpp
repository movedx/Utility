/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UtilityAudioProcessorEditor::UtilityAudioProcessorEditor(UtilityAudioProcessor& p)
    : AudioProcessorEditor(&p),
    audioProcessor(p),
    widthSlider([this](const juce::MouseEvent& e) { showWidthSliderContextMenu(e); }),
    midSideSlider([this](const juce::MouseEvent& e) { showWidthSliderContextMenu(e); })
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //setResizable(true, true);
    //setResizeLimits(300, 500, 600, 1000);

    //const float ratio = 0.6f;
    //getConstrainer()->setFixedAspectRatio(ratio);

    using namespace juce;

    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(fontRegular);

    setSize(300, 430);

    //setMouseTracking(true);
    //for (int i = 0; i < getNumChildComponents(); ++i)
    //{
    //    if (auto* child = getChildComponent(i))
    //        child->mouse
    //        child->setMouseTracking(true);
    //}

#if JUCE_DEBUG
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        if (auto* child = getChildComponent(i))
        {
            child->setInterceptsMouseClicks(true, true);
        }
    }
#endif

    juce::Font smallFont = Font(fontRegular);
    smallFont.setHeight(20.f);

    juce::Font mediumFont = Font(fontMedium);
    mediumFont.setHeight(20.f);

    juce::Font semiBoldFont = Font(fontSemiBold);
    semiBoldFont.setHeight(20.f);

    juce::Font boldFont = Font(fontBold);
    boldFont.setHeight(20.f);

    setName("Main Window");
    inputLabel.setName("Input Label");
    outputLabel.setName("Output Label");
    widthLabel.setName("Width Label");
    balanceLabel.setName("Balance Label");
    gainLabel.setName("Gain Label");
    invLeftPhaseButton.setName("Invert Left Phase Button");
    invRightPhaseButton.setName("Invert Right Phase Button");
    monoButton.setName("Mono Button");
    bassMonoButton.setName("Bass Mono Button");
    bassPreviewButton.setName("Bass Preview Button");
    muteButton.setName("Mute Button");
    dcButton.setName("DC Button");
    widthSlider.setName("Width Slider");
    gainSlider.setName("Gain Slider");
    balanceSlider.setName("Balance Slider");
    bassCrossoverSlider.setName("Bass Crossover Slider");
    modeComboBox.setName("Mode ComboBox");
    midSideSlider.setName("Mid/Side Balance");


    invLeftPhaseButton.setButtonText(u8"\u00D8 L");
    invRightPhaseButton.setButtonText(u8"\u00D8 R");
    monoButton.setButtonText("Mono");
    bassMonoButton.setButtonText("Bass Mono");

    bassPreviewButton.setButtonText(u8"\U0001F3A7");
    muteButton.setButtonText("Mute");
    dcButton.setButtonText("DC");

    widthLabel.setText("Width", NotificationType::dontSendNotification);
    widthLabel.setColour(Label::textColourId, juce::Colours::black);
    widthLabel.setJustificationType(Justification::centredBottom);
    widthLabel.setFont(semiBoldFont.withHeight(FontHeight::L));

    widthSlider.setLookAndFeel(&lnf);
    widthSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    widthSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    widthSlider.setTextValueSuffix("%");

    midSideLabel.setText("Mid/Side", NotificationType::dontSendNotification);
    midSideLabel.setColour(Label::textColourId, juce::Colours::black);
    midSideLabel.setJustificationType(Justification::centredBottom);
    midSideLabel.setFont(semiBoldFont.withHeight(FontHeight::L));

    midSideSlider.setLookAndFeel(&lnf);
    midSideSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    midSideSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    midSideSlider.setTextValueSuffix("<mid/side>");

    gainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    gainLabel.setColour(juce::Label::textColourId, Colours::black);
    gainLabel.setJustificationType(Justification::centredBottom);
    gainLabel.setFont(semiBoldFont.withHeight(FontHeight::L));

    gainSlider.setLookAndFeel(&lnf);
    gainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    gainSlider.setTextValueSuffix("dB");

    balanceLabel.setText("Balance", NotificationType::dontSendNotification);
    balanceLabel.setColour(Label::textColourId, Colours::black);
    balanceLabel.setJustificationType(Justification::centredBottom);
    balanceLabel.setFont(semiBoldFont.withHeight(FontHeight::L));


    balanceSlider.setLookAndFeel(&lnf);
    balanceSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    balanceSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    balanceSlider.setTextValueSuffix("<balance>");

    bassCrossoverSlider.setLookAndFeel(&lnf);
    bassCrossoverSlider.setSliderStyle(Slider::LinearHorizontal);
    bassCrossoverSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    bassCrossoverSlider.setTextValueSuffix("Hz");


    inputLabel.setColour(juce::Label::textColourId, Colours::black);
    inputLabel.setText("Input", NotificationType::dontSendNotification);
    inputLabel.setFont(boldFont.withHeight(FontHeight::XL));
    inputLabel.setJustificationType(Justification::centred);

    outputLabel.setColour(juce::Label::textColourId, Colours::black);
    outputLabel.setText("Output", NotificationType::dontSendNotification);
    outputLabel.setFont(boldFont.withHeight(FontHeight::XL));
    outputLabel.setJustificationType(Justification::centred);

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

    bassMonoButton.onClick = [this]() { onClickBassMono(); };
    onClickBassMono();

    midSideModePopupMenu.addItem(1, "Mid/Side Mode", true, midSideModeButton.getToggleState());

    addAndMakeVisible(invLeftPhaseButton);
    addAndMakeVisible(invRightPhaseButton);
    addAndMakeVisible(monoButton);
    addAndMakeVisible(bassMonoButton);
    addAndMakeVisible(bassPreviewButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(dcButton);

    addAndMakeVisible(gainSlider);
    addAndMakeVisible(balanceSlider);
    addAndMakeVisible(bassCrossoverSlider);
    
    addChildComponent(midSideSlider);
    addChildComponent(midSideLabel);

    addChildComponent(widthSlider);
    addChildComponent(widthLabel);


    updateWidthMidSideVisibility();


    addAndMakeVisible(modeComboBox);

    addAndMakeVisible(inputLabel);
    addAndMakeVisible(outputLabel);
    addAndMakeVisible(balanceLabel);
    addAndMakeVisible(gainLabel);

    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;

    widthSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "Width", widthSlider);
    midSideSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "MidSide", midSideSlider);
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

    midSideModeButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "MidSideMode", midSideModeButton);
}

UtilityAudioProcessorEditor::~UtilityAudioProcessorEditor()
{
    widthSlider.setLookAndFeel(nullptr);
    gainSlider.setLookAndFeel(nullptr);
    balanceSlider.setLookAndFeel(nullptr);
    bassCrossoverSlider.setLookAndFeel(nullptr);
    modeComboBox.setLookAndFeel(nullptr);
    midSideSlider.setLookAndFeel(nullptr);

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
    g.fillAll(juce::Colours::lightgrey);
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::grey.withAlpha(0.5f));
    g.drawVerticalLine(bounds.getWidth() / 2, bounds.getY() + 20, bounds.getBottom() - 20);
}

void UtilityAudioProcessorEditor::resized()
{

    auto area = getLocalBounds();
    auto left = area.withTrimmedRight(area.getWidth() / 2);
    auto right = area.withTrimmedLeft(area.getWidth() / 2);

    int itemHeight = 40;
    int knobHeight = itemHeight * 3;
    int itemMargin = 5;
    int padding = 5;

    inputLabel.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));

    auto buttonArea = left.removeFromTop(itemHeight).reduced(itemMargin);
    auto buttonWidth = buttonArea.getWidth() / 2;
    invLeftPhaseButton.setBounds(buttonArea.removeFromLeft(buttonWidth).reduced(padding));
    invRightPhaseButton.setBounds(buttonArea.reduced(padding));

    modeComboBox.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin).reduced(padding));

    midSideLabel.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin));
    midSideSlider.setBounds(left.removeFromTop(knobHeight).reduced(itemMargin));
    widthLabel.setBounds(midSideLabel.getBounds());
    widthSlider.setBounds(midSideSlider.getBounds());

    monoButton.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin).reduced(padding));


    left.removeFromTop(25.f);

    bassMonoButton.setBounds(left.removeFromTop(itemHeight).reduced(itemMargin).reduced(padding));

    auto bxAndPArea = left.removeFromTop(itemHeight).reduced(itemMargin);
    auto bxAndPWidth = bxAndPArea.getWidth() / 2;

    bassCrossoverSlider.setBounds(bxAndPArea.removeFromLeft(bxAndPWidth).reduced(padding));
    bassPreviewButton.setBounds(bxAndPArea.reduced(padding));

    //// 1. Get the initial area for the slider
    //auto sliderArea = bxAndPArea.removeFromLeft(bxAndPWidth);
    //// 2. Get the remaining initial area for the button
    //auto buttonArea2 = bxAndPArea; // This now holds the rest

    //// 3. Apply padding selectively to the slider area
    //// Trim top, left, bottom, but NOT right
    //sliderArea = sliderArea.withTrimmedTop(padding)
    //    .withTrimmedLeft(padding)
    //    .withTrimmedBottom(padding);
    //bassCrossoverSlider.setBounds(sliderArea);

    //// 4. Apply padding selectively to the button area
    //// Trim top, right, bottom, but NOT left
    //buttonArea2 = buttonArea2.withTrimmedTop(padding)
    //    .withTrimmedRight(padding)
    //    .withTrimmedBottom(padding);
    //bassPreviewButton.setBounds(buttonArea2);

    //bassCrossoverSlider.setBounds(bxAndPArea.removeFromLeft(bxAndPWidth).reduced(padding));
    //auto bcsBounds = bassCrossoverSlider.getBounds();
    ////bcsBounds.setWidth(bcsBounds.getWidth() + padding * 4);
    ////bassCrossoverSlider.setBounds(bcsBounds);
    //bassPreviewButton.setBounds(bxAndPArea.reduced(padding));

    outputLabel.setBounds(right.removeFromTop(itemHeight).reduced(itemMargin));

    gainLabel.setBounds(right.removeFromTop(itemHeight).reduced(itemMargin));
    gainSlider.setBounds(right.removeFromTop(knobHeight).reduced(itemMargin));

    right.removeFromTop(25.f);

    balanceLabel.setBounds(right.removeFromTop(itemHeight).reduced(itemMargin));
    balanceSlider.setBounds(right.removeFromTop(knobHeight).reduced(itemMargin));

    auto muteDcArea = right.removeFromTop(itemHeight).reduced(itemMargin);
    auto muteWidth = muteDcArea.getWidth() / 2;
    muteButton.setBounds(muteDcArea.removeFromLeft(muteWidth).reduced(padding));
    dcButton.setBounds(muteDcArea.reduced(padding));
#if JUCE_DEBUG
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        if (auto* child = getChildComponent(i))
        {
            DBG("Child Component: " + child->getName());
            DBG("Bounds: " + child->getBounds().toString());
        }
    }
#endif
}


void UtilityAudioProcessorEditor::updateWidthMidSideVisibility()
{
    bool isMidSide = midSideModeButton.getToggleState();
    DBG("isMidSide........................." + juce::String((int)isMidSide));

    widthLabel.setVisible(!isMidSide);
    widthSlider.setVisible(!isMidSide);

    midSideLabel.setVisible(isMidSide);
    midSideSlider.setVisible(isMidSide);

    // Optional: Request a repaint if visibility changes might affect drawing
     repaint(); // Usually not strictly needed as setVisible often triggers repaint
     DBG("updateWidthMidSideVisibility.........................");
}

void UtilityAudioProcessorEditor::mouseMove(const juce::MouseEvent& event)
{
#if JUCE_DEBUG
    //hoveredComponent = getComponentAt(event.getPosition());
    //if (hoveredComponent != nullptr)
    //{
    //    DBG("Hovered Component: " + hoveredComponent->getName());
    //    DBG("Hovered Component Type: " + juce::String(typeid(*hoveredComponent).name()));
    //}
    //else
    //{
    //    DBG("No component is being hovered over.");
    //}
#endif
}


bool UtilityAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
#if JUCE_DEBUG
    if (key.getTextCharacter() == 'P' && key.getModifiers().isShiftDown())
    {
        if (hoveredComponent != nullptr)
        {
            auto globalMousePos = juce::Desktop::getInstance().getMainMouseSource().getScreenPosition();

            auto localMousePos = hoveredComponent->getLocalPoint(nullptr, globalMousePos);

            DBG("Hovered Component: " + hoveredComponent->getName());
            DBG("Mouse Position Relative to Hovered Component: " + localMousePos.toString());
            DBG("LocalBounds of Hovered Component: " + hoveredComponent->getLocalBounds().toString());
        }
        else
        {
            DBG("No component is currently being hovered over.");
        }
        return true;
    }
    if (key.getTextCharacter() == 'p')
    {
        auto globalMousePos = juce::Desktop::getInstance().getMainMouseSource().getScreenPosition();
        auto localMousePos = getLocalPoint(nullptr, globalMousePos);
        //DBG("Global Mouse Position: " + globalMousePos.toString());
        DBG("Mouse Position: " + localMousePos.toString());
        return true;
    }
#endif
    return false;
}

void UtilityAudioProcessorEditor::onClickBassMono()
{
    if (bassMonoButton.getToggleStateValue().toString() == "1")
    {
        bassCrossoverSlider.setAlpha(1.f);
        bassCrossoverSlider.setEnabled(true);

        bassPreviewButton.setAlpha(1.f);
        bassPreviewButton.setEnabled(true);

    }
    else
    {
        bassCrossoverSlider.setAlpha(0.3f);
        bassCrossoverSlider.setEnabled(false);

        bassPreviewButton.setAlpha(0.3f);
        bassPreviewButton.setEnabled(false);

        if (bassPreviewButton.getToggleState())
        {
            bassPreviewButton.setToggleState(false, juce::NotificationType::sendNotification);
        }
    }
}


void UtilityAudioProcessorEditor::showWidthSliderContextMenu(const juce::MouseEvent& e)
{
    midSideModePopupMenu.clear();

    midSideModePopupMenu.addItem(1, "Mid/Side Mode", true, midSideModeButton.getToggleState());

    midSideModePopupMenu.showMenuAsync(juce::PopupMenu::Options().withTargetScreenArea(juce::Rectangle<int>(e.getScreenX(), e.getScreenY(), 1, 1)), [this](int result)
    {
        if (result == 1)
        {
            midSideModeButton.setToggleState(!midSideModeButton.getToggleState(), juce::NotificationType::sendNotification);
            DBG("Mid/Side Mode toggled to: " + juce::String(midSideModeButton.getToggleState() ? "ON" : "OFF"));
            updateWidthMidSideVisibility();

            //if (midSideModeButton.getToggleState())
            //{
            //    DBG("Toggle state is True, set visible false...");
            //    widthLabel.setVisible(false);
            //    midSideLabel.setVisible(true);
            //}
        }
    });
}