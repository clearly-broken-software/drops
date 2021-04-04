/* license */
#include "CheckBox.hpp"

START_NAMESPACE_DISTRHO

CheckBox::CheckBox(Window &parent) noexcept
    : NanoWidget(parent)
{
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    highlight_color = Color(1.0f, 0.5f, 0.5f);
    has_mouse_ = false;
    isActive = false;
}

CheckBox::CheckBox(Widget *widget) noexcept
    : NanoWidget(widget)
{
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    highlight_color = Color(1.0f, 0.5f, 0.5f);
    has_mouse_ = false;
    isActive = false;
}

bool CheckBox::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        isActive = !isActive;
        callback->onCheckBoxClicked(this, isActive);
        return true; // TODO check if this should be true/false
    }
    else
    {
        return false;
    }
}
bool CheckBox::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos) && !has_mouse_)
    {
        has_mouse_ = true; // TODO check if this should be true/false
    }
    repaint();

    if (!contains(ev.pos) && has_mouse_)
    {
        has_mouse_ = false;
        repaint();
    }
    return false;
}

void CheckBox::onNanoDisplay()
{
    float width = getWidth();
    float height = getHeight();
   
    // label
    fontFaceId(main_font_);
    fontSize(labelSize);
    Rectangle<float> bounds;
    textBounds(0.f, 0.f, label, NULL, bounds);
    const float label_height = bounds.getHeight();

    const float label_x = width * .5f; //- label_width / 2.0f;
    const float label_y = height - label_height;
    beginPath();
    fillColor(text_color);
    textAlign(ALIGN_CENTER | ALIGN_TOP);
    text(label_x, label_y, label, NULL);
    closePath();

    //box
    if (has_mouse_)
    {
        fill_color_ = highlight_color;
    }
    else if (isActive)
    {
        fill_color_ = foreground_color;
    }
    else
    {
        fill_color_ = background_color;
    }

    const float box_x = width / 2.f - boxSize / 2.f;
    const float box_y = height - label_height - boxSize - margin;
    beginPath();
    fillColor(fill_color_);
    roundedRect(box_x + 1.f, box_y + 1.f,
                boxSize - 2.f, boxSize - 2.f,
                2.f);
    fill();
    closePath();
}

void CheckBox::setCallback(Callback *cb)
{
    callback = cb;
}
void CheckBox::setFont(const char *fontName, const uchar *data, uint dataSize)
{
    main_font_ = createFontFromMemory(fontName, data, dataSize, false);
}

END_NAMESPACE_DISTRHO
