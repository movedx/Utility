#pragma once

#include <JuceHeader.h>


enum FontHeight : int
{
    S = 18,
    M = 20,
    L = 22,
    XL = 25
};


struct Fonts
{
    static juce::Typeface::Ptr regular;
    static juce::Typeface::Ptr medium;
    static juce::Typeface::Ptr semiBold;
    static juce::Typeface::Ptr bold;

    // Function to load typefaces (call this once)
    // We make this static so it doesn't require an instance of Fonts
    static void loadTypefaces()
    {
        // Load only if not already loaded
        if (regular == nullptr)
        {
            regular = juce::Typeface::createSystemTypefaceFor(BinaryData::MontserratRegular_ttf, BinaryData::MontserratRegular_ttfSize);
            jassert(regular != nullptr); // Ensure font loaded
        }
        if (medium == nullptr)
        {
            medium = juce::Typeface::createSystemTypefaceFor(BinaryData::MontserratMedium_ttf, BinaryData::MontserratMedium_ttfSize);
            jassert(medium != nullptr);
        }
        if (semiBold == nullptr)
        {
            semiBold = juce::Typeface::createSystemTypefaceFor(BinaryData::MontserratSemiBold_ttf, BinaryData::MontserratSemiBold_ttfSize);
            jassert(semiBold != nullptr);
        }
        if (bold == nullptr)
        {
            bold = juce::Typeface::createSystemTypefaceFor(BinaryData::MontserratBold_ttf, BinaryData::MontserratBold_ttfSize);
            jassert(bold != nullptr);
        }
    }

    static void unloadTypefaces()
    {
        regular = nullptr;
        medium = nullptr;
        semiBold = nullptr;
        bold = nullptr;
        juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(juce::Typeface::Ptr()); // Reset LnF too
        // If you set the LnF on your specific instance, reset that too:
        // myCustomLnfInstance->setDefaultSansSerifTypeface(nullptr); // If applicable
        DBG("Unloaded custom typefaces.");
    }

    // Helper function to get a juce::Font object
    // Example: get(Fonts::regular, FontHeight::M)
    static juce::Font get(const juce::Typeface::Ptr& typeface, FontHeight height)
    {
        jassert(typeface != nullptr); // Make sure typeface is loaded before use
        if (typeface != nullptr)
            return juce::Font(typeface).withHeight(static_cast<float>(height));
        else
            return juce::Font(static_cast<float>(height)); // Fallback to default font
    }

    // Convenience helpers for specific weights
    static juce::Font getRegular(FontHeight height) { return get(regular, height); }
    static juce::Font getMedium(FontHeight height) { return get(medium, height); }
    static juce::Font getSemiBold(FontHeight height) { return get(semiBold, height); }
    static juce::Font getBold(FontHeight height) { return get(bold, height); }

private:
    // Prevent instantiation of this utility struct
    Fonts() = delete;
};