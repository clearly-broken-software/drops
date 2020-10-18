/* license */
#include "Knob.hpp"

START_NAMESPACE_DISTRHO

Knob::Knob(Window &parent) noexcept
    : NanoWidget(parent)
{
    loadSharedResources();
    dragging_ = false;
    value_ = 0.f;
    valueTmp_ = 0.f;
    labelSize = 14.0f;
    label = "label";
    margin = 4.0f;
    fontFace(NANOVG_DEJAVU_SANS_TTF);
    font_ = findFont(NANOVG_DEJAVU_SANS_TTF);
    if (font_ == -1)
    {
        fprintf(stderr, "%s", "font not found\n");
    }
    foreground_color = Color(1, 1, 1);
    background_color = Color(0, 0, 0);
    text_color = Color(1, 1, 1);
    fill_color_ = foreground_color;
}

bool Knob::onMouse(const MouseEvent &ev)
{
    if (!ev.press && !contains(ev.pos) && !dragging_)
    {
        has_mouse_ = false;
        repaint();
        return false;
    }
    
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        dragging_ = true;
        mouseY_ = ev.pos.getY();
        return false;
    }
    else if (!ev.press && ev.button == 1 && dragging_)
    {
        dragging_ = false;
        repaint();
        return false;
    }
    else
    {
        return false;
    }
}

bool Knob::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos))
    {
        has_mouse_ = true;
        repaint();
    }
    else
    {
        if (!dragging_ && has_mouse_)
        {
            has_mouse_ = false;
            repaint();
        }
    }
    if (dragging_)
    {
        fill_color_ = highlite_color;
        float d, val = 0.0f;
        const int movY = mouseY_ - ev.pos.getY();
        d = 2000.0f;
        val = valueTmp_ + (1.f / d * float(movY));
        setValue(val);
        if (val > 1.0f || val < 0.0f)
        {
            mouseY_ = ev.pos.getY();
        }
    }
    return false;
}

void Knob::onNanoDisplay()
{
    const float height = getHeight();
    const float width = getWidth();

    const float stroke_width = 8.0f;

    // measure string
    fontFaceId(font_);
    fontSize(labelSize);
    Rectangle<float> bounds;
    textBounds(0.f, 0.f, label.c_str(), NULL, bounds);
    const float label_width = bounds.getWidth();
    const float label_height = bounds.getHeight();
    // label
    const float label_x = width / 2.0f; //- label_width / 2.0f;
    const float label_y = height - label_height;
    const float radius = (height - label_height - margin) / 2.0f;
    const float center_x = (width / 2.f);
    const float center_y = radius;
    beginPath();
    fillColor(text_color);
    textAlign(ALIGN_CENTER);
    text(label_x, label_y, label.c_str(), NULL);
    closePath();

    //Gauge (empty)
    beginPath();
    strokeWidth(stroke_width);
    strokeColor(background_color);
    arc(center_x, center_y, radius - stroke_width, 0.75f * M_PI, 0.25f * M_PI, NanoVG::Winding::CW);
    stroke();
    closePath();

    //Gauge (value)
    beginPath();
    strokeWidth(stroke_width);
    if (has_mouse_)
    {
        fill_color_ = highlite_color;
    }
    else
    {
        fill_color_ = foreground_color;
    }

    strokeColor(fill_color_);
    arc(center_x, center_y, radius - stroke_width, 0.75f * M_PI, (0.75f + 1.5f * value_) * M_PI, NanoVG::Winding::CW);
    stroke();
    closePath();

    // label
    // find center
    float centerY = getWidth() / 2.0f;
}

void Knob::setValue(float val) noexcept
{
    value_ = std::max(0.0f, std::min(val, 1.0f));
    valueTmp_ = value_;
    callback->knobValueChanged(this, value_);
}

void Knob::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
