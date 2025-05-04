
#include "ContextMenuSlider.h"

void ContextMenuSlider::mouseDown(const juce::MouseEvent& e)
{
    if (e.mods.isRightButtonDown() && rightClickCallback)
    {
        rightClickCallback(e);
    }
    else
    {
        juce::Slider::mouseDown(e);
    }
}
