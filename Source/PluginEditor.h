/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

enum FontHeight
{
    S = 18,
    M = 20,
    L = 22,
    XL = 25
};

class CompactSlider : public juce::Slider
{
    int offsetX = 0;

    void mouseDown(const juce::MouseEvent& e) override
    {
        offsetX = e.x - this->getPositionOfValue(getValue()); // relative Y position from thumb center

        //if (abs(offsetX) > 10) // is out of thumb area?
        //    offsetX = 0; // ignore offsetY

        juce::Slider::mouseDown(e.withNewPosition(juce::Point<int>(e.x - offsetX, e.y)));
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        juce::Slider::mouseDrag(e.withNewPosition(juce::Point<int>(e.x - offsetX, e.y)));
    }
};

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(4.0f);
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto center = bounds.getCentre();
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Draw arc
        g.setColour(juce::Colours::grey);
        juce::Path arc;
        arc.addCentredArc(center.x, center.y, radius, radius, 0.0f,
                          rotaryStartAngle, rotaryEndAngle, true);
        g.strokePath(arc, juce::PathStrokeType(4.0f));

        // Draw pointer
        float pointerLength = radius * 0.85f;
        float pointerThickness = 3.0f;
        juce::Point<float> startPoint = center.getPointOnCircumference(radius * 0.6f, angle);
        juce::Point<float> endPoint = center.getPointOnCircumference(pointerLength, angle);

        g.setColour(juce::Colours::black);
        g.drawLine({ startPoint, endPoint }, pointerThickness);

        juce::String suffix = makeSuffix(slider);

        // Draw value in the center of the knob
        //juce::Font monoBoldFont(juce::Font::getDefaultMonospacedFontName(), 15.0f, juce::Font::bold);
        //g.setFont(monoBoldFont);
        g.setColour(juce::Colours::black);
        g.setFont(FontHeight::M);
        auto valueText = juce::String(slider.getTextValueSuffix().compareIgnoreCase("<balance>") == 0 ? abs(slider.getValue()) : slider.getValue(), 0) + suffix;
        g.drawFittedText(valueText, bounds.toNearestInt(), juce::Justification::centred, 1);
    }

    juce::String makeSuffix(juce::Slider& slider)
    {
        juce::String suffix = slider.getTextValueSuffix();
        auto val = slider.getValue();

        if (suffix.compareIgnoreCase("<balance>") == 0)
        {
            suffix = "C"; // thin space
            if (val < 0)
            {
                suffix = "L";
            }
            else if (val > 0)
            {
                suffix = "R";
            }
            val = abs(val);
        }
        return juce::String(u8"\u2009") + suffix;
    }
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle, juce::Slider& slider) override
    {
        //slider.setVelocityBasedMode(true);
        int position = slider.getPositionOfValue(slider.getValue());
        //slider.setSliderSnapsToMousePosition(false);
        g.setColour(juce::Colours::blue.withAlpha(0.5f));
        g.fillRect(0, 0, int(sliderPos - x), height);

        g.setColour(juce::Colours::black);
        g.drawRect(0, 0, width, height, 1);

        juce::String suffix = makeSuffix(slider);

        g.setColour(juce::Colours::black);
        auto valueText = juce::String(slider.getValue(), 0) + suffix;
        g.drawFittedText(valueText, juce::Rectangle(0, 0, width, height), juce::Justification::centred, 1);
    }


    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override
    {
        auto bounds = juce::Rectangle<int>(0, 0, width, height);

        g.setColour(juce::Colours::whitesmoke);
        g.fillRect(bounds);

        g.setColour(juce::Colours::black);
        g.drawRect(bounds, 1);

        box.setColour(juce::ComboBox::textColourId, juce::Colours::black);


        juce::Path arrow;
        const float arrowSize = 6.0f;
        const float cx = static_cast<float>(buttonX + buttonW / 2);
        const float cy = static_cast<float>(buttonY + buttonH / 2);

        arrow.addTriangle(
            cx - arrowSize * 0.5f, cy - arrowSize * 0.3f,
            cx + arrowSize * 0.5f, cy - arrowSize * 0.3f,
            cx, cy + arrowSize * 0.4f
        );

        g.setColour(juce::Colours::black);
        g.fillPath(arrow);
    }

    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                           bool isSeparator, bool isActive, bool isHighlighted,
                           bool isTicked, bool hasSubMenu, const juce::String& text,
                           const juce::String& shortcutKeyText, const juce::Drawable* icon,
                           const juce::Colour* textColour) override
    {
        if (isSeparator)
        {
            g.setColour(juce::Colours::whitesmoke);
            g.drawLine((float)area.getX(), (float)(area.getCentreY()),
                       (float)area.getRight(), (float)(area.getCentreY()));
            return;
        }

        auto backgroundColour = isHighlighted ? juce::Colours::lightgrey : juce::Colours::whitesmoke;
        g.setColour(backgroundColour);
        g.fillRect(area);

        juce::Colour colourToUse = textColour ? *textColour
            : (isActive ? juce::Colours::black : juce::Colours::whitesmoke);

        g.setColour(colourToUse);
        g.drawFittedText(text, area.reduced(6), juce::Justification::centredLeft, 1);
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();


        g.setColour(button.getToggleState() ? juce::Colour(232, 171, 5) : juce::Colours::grey.brighter(0.3f));
        g.fillRect(bounds);

        g.setColour(juce::Colours::black);
        g.drawRect(bounds);

        g.setColour(juce::Colours::black);
        g.drawFittedText(button.getButtonText(), bounds.toNearestInt(), juce::Justification::centred, 1);
    }
};


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

    void mouseMove(const juce::MouseEvent& event) override;

    bool keyPressed(const juce::KeyPress& key) override;

private:
    void onClickBassMono();

    //juce::Typeface::Ptr fontRegular = juce::Typeface::createSystemTypefaceFor(BinaryData::GyrochromeRegular_otf, BinaryData::GyrochromeRegular_otfSize);
    //juce::Typeface::Ptr fontMedium = juce::Typeface::createSystemTypefaceFor(BinaryData::GyrochromeMedium_otf, BinaryData::GyrochromeMedium_otfSize);
    //juce::Typeface::Ptr fontSemiBold = juce::Typeface::createSystemTypefaceFor(BinaryData::GyrochromeSemiBold_otf, BinaryData::GyrochromeSemiBold_otfSize);
    //juce::Typeface::Ptr fontBold = juce::Typeface::createSystemTypefaceFor(BinaryData::GyrochromeBold_otf, BinaryData::GyrochromeBold_otfSize);

    //juce::Typeface::Ptr fontRegular = juce::Typeface::createSystemTypefaceFor(BinaryData::Exo2Regular_ttf, BinaryData::Exo2Regular_ttfSize);
    //juce::Typeface::Ptr fontMedium = juce::Typeface::createSystemTypefaceFor(BinaryData::Exo2Medium_ttf, BinaryData::Exo2Medium_ttfSize);
    //juce::Typeface::Ptr fontSemiBold = juce::Typeface::createSystemTypefaceFor(BinaryData::Exo2SemiBold_ttf, BinaryData::Exo2SemiBold_ttfSize);
    //juce::Typeface::Ptr fontBold = juce::Typeface::createSystemTypefaceFor(BinaryData::Exo2Bold_ttf, BinaryData::Exo2Bold_ttfSize);

    //juce::Typeface::Ptr fontRegular = juce::Typeface::createSystemTypefaceFor(BinaryData::OrbitronRegular_ttf, BinaryData::OrbitronRegular_ttfSize);
    //juce::Typeface::Ptr fontMedium = juce::Typeface::createSystemTypefaceFor(BinaryData::OrbitronMedium_ttf, BinaryData::OrbitronMedium_ttfSize);
    //juce::Typeface::Ptr fontSemiBold = juce::Typeface::createSystemTypefaceFor(BinaryData::OrbitronSemiBold_ttf, BinaryData::OrbitronSemiBold_ttfSize);
    //juce::Typeface::Ptr fontBold = juce::Typeface::createSystemTypefaceFor(BinaryData::OrbitronBold_ttf, BinaryData::OrbitronBold_ttfSize);

    juce::Typeface::Ptr fontRegular = juce::Typeface::createSystemTypefaceFor(BinaryData::MontserratRegular_ttf, BinaryData::MontserratRegular_ttfSize);
    juce::Typeface::Ptr fontMedium = juce::Typeface::createSystemTypefaceFor(BinaryData::MontserratMedium_ttf, BinaryData::MontserratMedium_ttfSize);
    juce::Typeface::Ptr fontSemiBold = juce::Typeface::createSystemTypefaceFor(BinaryData::MontserratSemiBold_ttf, BinaryData::MontserratSemiBold_ttfSize);
    juce::Typeface::Ptr fontBold = juce::Typeface::createSystemTypefaceFor(BinaryData::MontserratBold_ttf, BinaryData::MontserratBold_ttfSize);

    UtilityAudioProcessor& audioProcessor;

    LookAndFeel lnf;

    juce::Label inputLabel, outputLabel;
    juce::Label widthLabel, balanceLabel, gainLabel;

    juce::ToggleButton invLeftPhaseButton,
        invRightPhaseButton,
        monoButton,
        bassMonoButton,
        bassPreviewButton,
        muteButton,
        dcButton;

    juce::Slider widthSlider,
        gainSlider,
        balanceSlider;

    CompactSlider bassCrossoverSlider;

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

    juce::Component* hoveredComponent = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityAudioProcessorEditor)
};