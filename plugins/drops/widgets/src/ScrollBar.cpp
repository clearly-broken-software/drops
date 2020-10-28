/* license */
#include "ScrollBar.hpp"

START_NAMESPACE_DISTRHO

ScrollBar::ScrollBar(Window &parent) noexcept
    : NanoWidget(parent)
{
    dragging = false;
}

bool ScrollBar::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        dragging = true;
        callback->onScrollBarClicked(this, dragging);
        return false;
    }
    else if (!ev.press && ev.button == 1 && dragging)
    {
        dragging = false;
        callback->onScrollBarClicked(this, dragging);
        return false;
    }
    else
    {
        return false;
    }
}

void ScrollBar::onNanoDisplay()
{
    #ifdef DEBUG   
    int w = getWidth();
    int h = getHeight();
    beginPath();
    strokeColor(255, 0, 0);
    strokeWidth(1.0f);
    rect(1, 1, w-2, h-2);
    stroke();
    closePath();
    #endif
    
}

void ScrollBar::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
