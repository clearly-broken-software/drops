/* license */
#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

TextButton::TextButton(Window &parent) noexcept
    : NanoWidget(parent)
{
    buttonText = "BUTTON";
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    loadSharedResources();
    has_mouse_ = false;
}

TextButton::TextButton(Widget *widget) noexcept
    : NanoWidget(widget)
{
    buttonText = "BUTTON";
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    loadSharedResources();
    has_mouse_ = false;
    isActive = false;
}

bool TextButton::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        callback->onTextButtonClicked(this);
        return true;
    }
    else
    {
        return false;
    }
}
bool TextButton::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos) && !has_mouse_)
    {
        has_mouse_ = true;
        repaint();
    }
    if (!contains(ev.pos) && has_mouse_)
    {
        has_mouse_ = false;
        repaint();
    }
    return false;
}

void TextButton::onNanoDisplay()
{
    int width = getWidth();
    int height = getHeight();
    beginPath();
    fillColor(background_color);
    roundedRect(0, 0, width, height, 5);
    fill();
    closePath();
    beginPath();
    fillColor(background_color);
    rect(0, 5, width, height - 10);
    fill();
    closePath();

    // text
    beginPath();
    fillColor(foreground_color);
    fontSize(font_size);
    textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
    text(std::round(width / 2), std::round(height / 2.0f), buttonText.c_str(), NULL);
    closePath();

    // indicator
    if (has_mouse_ || isActive)
    {
        beginPath();
        lineCap(DGL::NanoVG::LineCap::ROUND);
        strokeColor(highlight_color);
        strokeWidth(4);
        moveTo(4, 4);
        lineTo(width - 4, 4);
        stroke();
        closePath();
    }
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
