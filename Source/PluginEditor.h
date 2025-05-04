/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#define ENABLE_INSPECTOR false

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "ContextMenuSlider.h"
#include "Font.h"

#if ENABLE_INSPECTOR
#include "melatonin_inspector/melatonin_inspector.h"
#endif

class UtilityAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:

    UtilityAudioProcessorEditor (UtilityAudioProcessor&);
    ~UtilityAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void updateWidthMidSideVisibility();

    bool keyPressed(const juce::KeyPress& key) override;

private:
#if ENABLE_INSPECTOR
    melatonin::Inspector inspector{ *this };
#endif
    void onClickBassMono();

    void showWidthSliderContextMenu(const juce::MouseEvent& e);

    UtilityAudioProcessor& audioProcessor;

    CustomLookAndFeel lnf;

    juce::Label inputLabel, outputLabel;
    juce::Label widthLabel, balanceLabel, gainLabel, midSideLabel;

    juce::ToggleButton invLeftPhaseButton,
        invRightPhaseButton,
        monoButton,
        bassMonoButton,
        bassPreviewButton,
        muteButton,
        dcButton;

    ContextMenuSlider widthSlider, midSideSlider;
    juce::Slider gainSlider, balanceSlider, bassCrossoverSlider;

    juce::ComboBox modeComboBox;

    juce::ToggleButton midSideModeButton;

    juce::PopupMenu midSideModePopupMenu;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midSideModeButtonAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> invLeftPhaseButtonAttachment,
        invRightPhaseButtonAttachment,
        monoButtonAttachment,
        bassMonoButtonAttachment,
        bassPreviewButtonAttachment,
        muteButtonAttachment,
        dcButtonAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthSliderAttachment,
        gainSliderAttachment,
        balanceSliderAttachment,
        bassCrossoverSliderAttachment,
        midSideSliderAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeComboBoxAttachment;

    juce::Component* hoveredComponent = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityAudioProcessorEditor)
};