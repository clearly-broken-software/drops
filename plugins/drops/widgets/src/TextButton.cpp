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
    int width = getWidth();
    int height = getHeight();
    beginPath();
    fillColor(169, 169, 169);
    rect(0, 0, width, height);
    fill();
    closePath();
    // text
    beginPath();
    fillColor(0, 0, 0);
    fontSize(24); 
    textAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    Rectangle<float> bounds;
    textBounds(0, 0, buttonText.c_str(), NULL, bounds);
    std::string tempText = buttonText;
    for (int i = 0; i < buttonText.size(); i++) // maybe i = 1 ??
    {
        textBounds(0, 0, tempText.c_str(), NULL, bounds);
        // too large ?
        if (bounds.getWidth() > width)
        {
            // remove 1st character
            tempText = buttonText.substr(i);
        }
        else
            break;
    }

    text(0, std::round(height / 2.0f), tempText.c_str(), NULL);
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
