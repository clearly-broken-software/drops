/* license */
#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

TextButton::TextButton(Window &parent, Size<uint> size) noexcept
    : NanoWidget(parent)
{
    printf("TextButton constructed\n");
    loadSharedResources();
}

bool TextButton::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        //  printf("mouse click %i in TextButton\n", ev.button);
        callback->textButtonClicked(this);
        return true;
    }
    else
    {
        return false;
    }
}

void TextButton::onNanoDisplay()
{
    printf("draw button\n");
    int w = getWidth();
    int h = getHeight();
    beginPath();
    fillColor(169, 169, 169);
    rect(0, 0, w, h);
    fill();
    closePath();
    // text
    beginPath();
    fillColor(0, 0, 0);
    fontSize(48);
    textAlign(ALIGN_LEFT | ALIGN_TOP);
    text(0, 0, "click to load sample", NULL);
    closePath();
}

void TextButton::setCallback(Callback *cb) 
{
    printf("setCallback\n");
    callback = cb;
}

END_NAMESPACE_DISTRHO
