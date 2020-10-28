/* license */
#include "RadioButton.hpp"

START_NAMESPACE_DISTRHO

RadioButton::RadioButton(Window &parent) noexcept
    : NanoWidget(parent)
{
    loadSharedResources();
    font_size = 14.f;
    margin = 2.f;
    background_color = Color(0.0f, 0.0f, 0.0f);
    foreground_color = Color(0.5f, 0.5f, 0.5f);
    highlight_color = Color(1.0f, 0.0f, 0.0f);
    text_color = Color(1.0f, 1.0f, 1.0f);
    active_option = 0;
}
RadioButton::RadioButton(Widget *widget) noexcept
    : NanoWidget(widget)
{
    loadSharedResources();
    font_size = 14.f;
    margin = 2.f;
    background_color = Color(0.0f, 0.0f, 0.0f);
    foreground_color = Color(0.5f, 0.5f, 0.5f);
    highlight_color = Color(1.0f, 0.0f, 0.0f);
    text_color = Color(1.0f, 1.0f, 1.0f);
    active_option = 0;
}

bool RadioButton::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {

        callback_->onRadioButtonClicked(this);
        return true;
    }
    else
    {
        return false;
    }
}

bool RadioButton::onMotion(const MotionEvent &ev)
{
    return false;
}

void RadioButton::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();
    const uint x = getAbsoluteX();
    const uint y = getAbsoluteY();
    printf("x %i,y %i, w %f, h %f\n",x,y,width,height);
    // background
    beginPath();
    fillColor(background_color);
    rect(0, 0, width, height);
    fill();
    closePath();

    strokeColor(foreground_color);
    Color fill_color = background_color;
    strokeWidth(margin);
    const float lineHeight = font_size + 2 * margin;
    float step_y = 0.0f;
    const float cx = margin + (font_size / 2.0f);
    const float cy = lineHeight / 2.0f;
    for (int i = 0; i < options.size(); i++)
    {
        beginPath();
        circle(cx, cy, font_size / 2.0f);
        fill_color = i == active_option ? highlight_color : background_color;
        fillColor(fill_color);
        stroke();
        fill();
        closePath();
        fillColor(text_color);
        fontSize(font_size);
        textAlign(ALIGN_LEFT | ALIGN_MIDDLE);
        beginPath();
        text(lineHeight, step_y + lineHeight / 2.0f, options[i], nullptr);
        closePath();
        step_y += lineHeight;
    }
}

void RadioButton::addOption(char *op)
{
    options.push_back(op);
}

void RadioButton::setCallback(Callback *cb)
{
    callback_ = cb;
}

END_NAMESPACE_DISTRHO
