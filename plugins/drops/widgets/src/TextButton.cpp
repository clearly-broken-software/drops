/* license */
#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

TextButton::TextButton(Window &parent, Size<uint> size) noexcept
    : NanoWidget(parent)
{
    buttonText = "click to load sample";
    loadSharedResources();
}

bool TextButton::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
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
    fontSize(24);
    textAlign(ALIGN_LEFT | ALIGN_TOP);
    text(0, 0, buttonText.c_str(), NULL);
    closePath();
}

void TextButton::setText(std::string str)
{
    buttonText = str;
}
void TextButton::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
