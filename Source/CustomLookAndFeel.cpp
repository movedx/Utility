
#include "CustomLookAndFeel.h"
#include "Font.h"


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
    if (suffix.compareIgnoreCase("<mid/side>") == 0)
    {
        suffix = ""; // thin space
        if (val < 0)
        {
            suffix = "M";
        }
        else if (val > 0)
        {
            suffix = "S";
        }
        val = abs(val);
    }
    return juce::String(u8"\u2009") + suffix;
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                        float sliderPosProportional, float rotaryStartAngle,
                        float rotaryEndAngle, juce::Slider& slider)
{
    float visualSliderPosProportional = sliderPosProportional;

    // Specific mapping for Width slider
    if (slider.getName() == "Width Slider")
    {
        auto currentValue = slider.getValue();
        auto minValue = slider.getMinimum(); // 0.0
        auto maxValue = slider.getMaximum(); // 400.0
        auto centerValue = 100.0; // The value we want at the 12 o'clock position

        if (currentValue <= centerValue)
        {
            visualSliderPosProportional = juce::jmap(currentValue, minValue, centerValue, 0.0, 0.5);
        }
        else
        {
            visualSliderPosProportional = juce::jmap(currentValue, centerValue, maxValue, 0.5, 1.0);
        }
    }

    auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(4.0f);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto center = bounds.getCentre();
    auto arcThickness = 4.0f; // Define arc thickness

    // Calculate the angle corresponding to the *visual* position
    auto angle = rotaryStartAngle + visualSliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    auto centerAngle = (rotaryStartAngle + rotaryEndAngle) / 2.0f; // Angle at 12 o'clock

    // --- Define Colors ---
    auto backgroundColour = juce::Colours::grey.withAlpha(0.7f);
    auto activeColour = juce::Colour(0, 238, 255); // Your active color

    // --- Draw the full background arc (inactive part) ---
    g.setColour(backgroundColour);
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(center.x, center.y, radius, radius, 0.0f,
                                rotaryStartAngle, rotaryEndAngle, true);
    // Using 'butt' end caps for the background arc might look cleaner if overlapping the active arc
    g.strokePath(backgroundArc, juce::PathStrokeType(arcThickness, juce::PathStrokeType::curved, juce::PathStrokeType::butt));

    // --- Determine if it's a centered slider ---
    bool isCenteredSlider = (slider.getTextValueSuffix().compareIgnoreCase("<balance>") == 0 ||
                                slider.getTextValueSuffix().compareIgnoreCase("<mid/side>") == 0 ||
                                slider.getName() == "Width Slider" ||
                                slider.getName() == "Gain Slider");

    // --- Draw the active arc portion ---
    juce::Path activeArc;
    g.setColour(activeColour);

    if (isCenteredSlider)
    {
        if (!juce::approximatelyEqual(angle, centerAngle)) // Avoid drawing if exactly at center
        {
            if (angle > centerAngle) // Value is to the right of center
            {
                activeArc.addCentredArc(center.x, center.y, radius, radius, 0.0f,
                                        centerAngle, angle, true);
            }
            else // Value is to the left of center
            {
                activeArc.addCentredArc(center.x, center.y, radius, radius, 0.0f,
                                        angle, centerAngle, true);
            }
        }
    }
    else // Normal slider (e.g., Gain) - fills from the start
    {
        if (!juce::approximatelyEqual(angle, rotaryStartAngle)) // Avoid drawing if exactly at start
        {
            activeArc.addCentredArc(center.x, center.y, radius, radius, 0.0f,
                                    rotaryStartAngle, angle, true);
        }
    }

    // Draw the active arc if it has a meaningful length
    if (!activeArc.isEmpty())
        // Use rounded ends for the active part for a nicer look
        g.strokePath(activeArc, juce::PathStrokeType(arcThickness, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));


    // --- Draw Pointer (Original Style) ---
    float pointerLength = radius * 0.85f;
    float pointerThickness = 3.0f;
    // *** REVERTED START POINT CALCULATION ***
    juce::Point<float> startPoint = center.getPointOnCircumference(radius * 0.6f, angle);
    juce::Point<float> endPoint = center.getPointOnCircumference(pointerLength, angle);

    g.setColour(juce::Colours::black); // Pointer color
    g.drawLine({ startPoint, endPoint }, pointerThickness);


    // --- Draw Text ---
    juce::String suffix = makeSuffix(slider);
    g.setColour(juce::Colours::black); // Text color
    g.setFont(FontHeight::M);

    auto value = 0.f;
    if (slider.getTextValueSuffix().compareIgnoreCase("<balance>") == 0 ||
        slider.getTextValueSuffix().compareIgnoreCase("<mid/side>") == 0)
    {
        value = std::abs(slider.getValue());
    }
    else
    {
        value = slider.getValue();
    }
    auto valueText = juce::String(value, 0) + suffix;
    g.drawFittedText(valueText, bounds.toNearestInt(), juce::Justification::centred, 1);
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                        float sliderPos, float minSliderPos, float maxSliderPos,
                        juce::Slider::SliderStyle, juce::Slider& slider)
{
    DBG("drawLinearSlider.x" << x);
    DBG("drawLinearSlider.y" << y);
    DBG("drawLinearSlider.width" << width);
    DBG("drawLinearSlider.height" << height);

    auto localBounds = slider.getLocalBounds();


    DBG("dlSlocalBounds" << localBounds.toString());
    DBG("sliderPos" << sliderPos);

    slider.setVelocityBasedMode(true);

    if (slider.getSliderStyle() == juce::Slider::SliderStyle::LinearHorizontal)
    {
        sliderPos = juce::jmap(int(sliderPos), x, width, localBounds.getX(), localBounds.getWidth());

        //int position = slider.getPositionOfValue(slider.getValue());
        //slider.setSliderSnapsToMousePosition(false);
        g.setColour(juce::Colours::blue.withAlpha(0.5f));
        g.fillRect(0, 0, int(sliderPos), height);
    }
    else if (slider.getSliderStyle() == juce::Slider::SliderStyle::LinearVertical)
    {
        //double current_value = slider.getValue();
        //sliderPos = juce::jmap(int(current_value), 20, 500, localBounds.getX(), localBounds.getWidth());

        //g.setColour(juce::Colours::blue.withAlpha(0.5f));
        //DBG("CURRENT VALUE: " << current_value);
        //DBG("CURRENT sliderPos: " << sliderPos);

        //g.fillRect(0, 0, int(sliderPos), height);
    }

    g.setColour(juce::Colours::black);
    g.drawRect(localBounds, 1);

    juce::String suffix = makeSuffix(slider);

    g.setColour(juce::Colours::black);
    auto valueText = juce::String(slider.getValue(), 0) + suffix;
    g.drawFittedText(valueText, localBounds, juce::Justification::centred, 1);
}


void CustomLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                    int buttonX, int buttonY, int buttonW, int buttonH,
                    juce::ComboBox& box)
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

void CustomLookAndFeel::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                        bool isSeparator, bool isActive, bool isHighlighted,
                        bool isTicked, bool hasSubMenu, const juce::String& text,
                        const juce::String& shortcutKeyText, const juce::Drawable* icon,
                        const juce::Colour* textColour)
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

void CustomLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();


    g.setColour(button.getToggleState() ? juce::Colour(232, 171, 5) : juce::Colours::grey.brighter(0.3f));
    g.fillRect(bounds);

    g.setColour(juce::Colours::black);
    g.drawRect(bounds);

    g.setColour(juce::Colours::black);
    g.drawFittedText(button.getButtonText(), bounds.toNearestInt(), juce::Justification::centred, 1);
}

