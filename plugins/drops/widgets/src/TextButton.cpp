/* license */
#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

TextButton::TextButton(Window &parent, Size<uint> size) noexcept
    : NanoWidget(parent)
{
    buttonText = "click to load sample";
    back_ground_color = Color(0.8f, 0.8f, 0.8f);
    text_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    loadSharedResources();
}

TextButton::TextButton(Widget * widget, Size<uint> size) noexcept
    : NanoWidget(widget)
{
    buttonText = "click to load sample";
    back_ground_color = Color(0.8f, 0.8f, 0.8f);
    text_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
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
    fillColor(back_ground_color);
    rect(0, 0, width, height);
    fill();
    closePath();

    // text
    beginPath();
    fillColor(text_color);
    fontSize(font_size);
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
