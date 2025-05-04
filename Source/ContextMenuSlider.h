
#pragma once

#include <JuceHeader.h>


class ContextMenuSlider : public juce::Slider
{
public:
    ContextMenuSlider(const std::function<void(const juce::MouseEvent&)>& onRightClickCallback) : rightClickCallback(onRightClickCallback) {}

    void mouseDown(const juce::MouseEvent& e) override;

private:
    std::function<void(const juce::MouseEvent&)> rightClickCallback;
};