/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct PlaceHolder : public juce::Component {
    PlaceHolder(const juce::String& _title, bool _gap) : Component(_title) { title = _title; gap = _gap; }
    ~PlaceHolder() {}
    void paint(juce::Graphics& g) override {
        if (!gap) {
            g.setColour(juce::Colour(24, 120, 41));
            g.fillRoundedRectangle(getLocalBounds().toFloat(), 5);
        }
        g.setColour(juce::Colours::black);
        g.drawText(title, getLocalBounds(), juce::Justification::centred);
    }
    void mouseDown(const juce::MouseEvent& event) override {
        DBG("Width: " + juce::String(getWidth()));
    }
    juce::String title;
    bool gap;
};

//==============================================================================
/**
*/
class UtilityAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    UtilityAudioProcessorEditor (UtilityAudioProcessor&);
    ~UtilityAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    UtilityAudioProcessor& audioProcessor;

    juce::Label inputLabel, outputLabel;

    juce::ToggleButton invLeftPhaseButton,
        invRightPhaseButton,
        monoButton,
        bassMonoButton,
        bassPreviewButton,
        muteButton,
        dcButton;

    juce::Slider widthSlider,
        gainSlider,
        balanceSlider,
        bassCrossoverSlider;

    juce::ComboBox modeComboBox;

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
        bassCrossoverSliderAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeComboBoxAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityAudioProcessorEditor)
};
