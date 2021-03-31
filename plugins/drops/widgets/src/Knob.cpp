/* license */
#include "Knob.hpp"

START_NAMESPACE_DISTRHO

Knob::Knob(Window &parent) noexcept
    : NanoWidget(parent)
{
    loadSharedResources();
    parent.addIdleCallback(this);
    dragging_ = false;
    has_mouse_ = false;
    value_ = 0.f;
    tmp_value_ = 0.f;
    max = 1.0f;
    min = 0.0f;
    real_min = 0.0f;
    real_max = 1.0f;
    using_log = false;
    is_centered = false;
    labelSize = 14.0f;
    label = "label";
    margin = 4.0f;
    gauge_width = 8.0f;
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
    popUp = nullptr;
    format_str = "%.2f";
    is_counting_down_ = false;
    countdown_ = 30;
}

Knob::Knob(Widget *parent) noexcept
    : NanoWidget(parent)
{
    loadSharedResources();
    parent->getParentWindow().addIdleCallback(this);
    dragging_ = false;
    has_mouse_ = false;
    value_ = 0.f;
    tmp_value_ = 0.f;
    max = 1.0f;
    min = 0.0f;
    real_min = 0.0f;
    real_max = 1.0f;
    using_log = false;
    is_centered = false;
    labelSize = 14.0f;
    label = "label";
    margin = 4.0f;
    gauge_width = 8.0f;
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
    popUp = nullptr;
    format_str = "%.2f";
    is_counting_down_ = false;
    countdown_ = 30;
}

float Knob::getValue() noexcept
{
    return value_;
}
bool Knob::onMouse(const MouseEvent &ev)
{
    if (!isVisible())
        return false;

    if (ev.button != 1)
        return false;

    if (ev.press)
    {
        if (!contains(ev.pos))
            return false;

        if ((ev.mod & kModifierShift) != 0 && using_default_)
        {
            setValue(default_value, true);
            tmp_value_ = value_;
            return false;
        }
        has_mouse_ = true;
        dragging_ = true;
        last_mouse_x_ = ev.pos.getX();
        last_mouse_y_ = ev.pos.getY();

        if (callback != nullptr)
        {
            callback->knobDragStarted(this);
        }
        if (popUp != nullptr)
        {
            int y = getAbsoluteY() + getHeight();
            popUp->setAbsoluteY(y);
            updatePopUp();
            popUp->show();
        }
        repaint();
        return false;
    }
    else if (dragging_)
    {
        float normValue = (value_ - min) / (max - min);

        if (callback != nullptr)
            callback->knobDragFinished(this, normValue);
        if (popUp != nullptr && !contains(ev.pos))
        {
            popUp->hide();
            is_counting_down_ = false;
        }
        dragging_ = false;
        repaint();
        return false;
    }
    has_mouse_ = false;
    return false;
}

bool Knob::onScroll(const ScrollEvent &ev)
{
    if (!isVisible())
        return false;
    if (!contains(ev.pos))
        return false;

    const float d = (ev.mod & kModifierControl) ? 2000.0f : 200.0f;
    float value = (using_log ? _invlogscale(tmp_value_) : tmp_value_) + (float(max - min) / d * 10.f * ev.delta.getY());

    if (using_log)
        value = _logscale(value);

    if (value < min)
    {
        tmp_value_ = value = min;
    }
    else if (value > max)
    {
        tmp_value_ = value = max;
    }
    printf("scroll knob value %f\n", value);
    setValue(value, true);
    float normValue = (value_ - min) / (max - min);
    callback->knobDragFinished(this, normValue);
    return false;
}

bool Knob::onMotion(const MotionEvent &ev)
{
    if (!isVisible())
        return false;

    if (contains(ev.pos) && !has_mouse_)
        has_mouse_ = true;

    // if the popup is visible it is opened by another widget
    if (has_mouse_ && (popUp != nullptr) && (!popUp->isVisible()))
    {
        is_counting_down_ = true;
        countdown_ = 30;
    };

    if (!contains(ev.pos) && !dragging_)
    {
        has_mouse_ = false;
        if (is_counting_down_)
        {
            is_counting_down_ = false;
            if (popUp != nullptr)
                popUp->hide();
        }
    }
    repaint();

    if (!dragging_)
    {
        return false;
    }

    float d, value = 0.0f;
    const int movY = last_mouse_y_ - ev.pos.getY();
    d = (ev.mod & kModifierControl) ? 2000.0f : 200.0f;
    value = (using_log ? _invlogscale(tmp_value_) : tmp_value_) + (float(max - min) / d * float(movY));

    if (using_log)
        value = _logscale(value);

    if (value < min)
    {
        tmp_value_ = value = min;
    }
    else if (value > max)
    {
        tmp_value_ = value = max;
    }

    setValue(value, false);

    last_mouse_x_ = ev.pos.getX();
    last_mouse_y_ = ev.pos.getY();

    return false;
}

void Knob::idleCallback()
{
    // are we in a countdown
    if (!is_counting_down_)
        return;

    // yes, start countdown
    countdown_--;
    // if < 0 show popUp
    if (countdown_ < 0)
    {
        //  printf("show popup\n");
        int y = getAbsoluteY() + getHeight();
        popUp->setAbsoluteY(y);
        popUp->background_color = background_color;
        popUp->foreground_color = foreground_color;
        popUp->text_color = text_color;
        updatePopUp();
        popUp->show();
    }
}

void Knob::onNanoDisplay()
{
    const float height = getHeight();
    const float width = getWidth();
    float normValue = (((using_log ? _invlogscale(value_) : value_) - min) / (max - min));
    if (normValue < 0.0f)
        normValue = 0.0f;

    //beginPath();
    // fillColor(Color(127,127,127));
    // rect(0,0,width,height);
    // fill();
    // closePath();

    // measure string
    fontFaceId(font_);
    fontSize(labelSize);
    Rectangle<float> bounds;
    textBounds(0.f, 0.f, label.c_str(), NULL, bounds);
    const float label_height = bounds.getHeight();

    // label
    const float label_x = width * .5f; //- label_width / 2.0f;
    const float label_y = height - label_height;
    const float radius = (height - label_height - margin) / 2.0f;
    const float center_x = (width * .5f);
    const float center_y = radius + margin;
    beginPath();
    fillColor(text_color);
    textAlign(ALIGN_CENTER | ALIGN_TOP);
    text(label_x, label_y, label.c_str(), NULL);
    closePath();

    //Gauge (empty)
    beginPath();
    strokeWidth(gauge_width);
    strokeColor(background_color);
    arc(center_x, center_y, radius - gauge_width / 2, 0.75f * M_PI, 0.25f * M_PI, NanoVG::Winding::CW);
    stroke();
    closePath();

    //Gauge (value)
    beginPath();
    strokeWidth(gauge_width);
    if (has_mouse_)
    {
        fill_color_ = highlight_color;
    }
    else
    {
        fill_color_ = foreground_color;
    }
    strokeColor(fill_color_);
    if (is_centered)
    {
        //   const float stop_angle = normValue > 0.5f ? (0.75 + 1.5f * normValue) * M_PI : (0.75f + normValue) * M_PI;
        const NanoVG::Winding w = normValue > 0.5f ? NanoVG::Winding::CW : NanoVG::Winding::CCW;
        arc(center_x,
            center_y,
            radius - gauge_width / 2,
            1.5f * M_PI,
            (0.75f + 1.5f * normValue) * M_PI,
            w);
    }
    else
    {
        arc(center_x,
            center_y,
            radius - gauge_width / 2,
            0.75f * M_PI,
            (0.75f + 1.5f * normValue) * M_PI,
            NanoVG::Winding::CW);
    }
    stroke();
    closePath();
    // if centered draw tickmark at top
    if (is_centered)
    {
        beginPath();
        arc(center_x, center_y,
            radius - gauge_width / 2,
            1.48f * M_PI,
            1.52f * M_PI,
            NanoVG::Winding::CW);
        stroke();
        closePath();
    }
}
void Knob::setValue(float val, bool sendCallback) noexcept
{
    value_ = std::max(min, std::min(val, max));
    tmp_value_ = value_;
    float normValue = (value_ - min) / (max - min);
    // float normValue = (((using_log ? _invlogscale(value_) : value_) - min) / (max - min));
    // if (normValue < 0.0f)
    //     normValue = 0.0f;
    if (popUp != nullptr)
        updatePopUp();

    if (sendCallback && callback != nullptr)
    {
        callback->knobValueChanged(this, normValue);
    }
}

float Knob::_logscale(float value) const
{
    const float b = std::log(max / min) / (max - min);
    const float a = max / std::exp(max * b);
    return a * std::exp(b * value);
}

float Knob::_invlogscale(float value) const
{
    const float b = std::log(max / min) / (max - min);
    const float a = max / std::exp(max * b);
    return std::log(value / a) / b;
}

void Knob::setCallback(Callback *cb)
{
    callback = cb;
}

void Knob::setPopUp(PopUp *p)
{
    popUp = p;
    popUp->background_color = background_color;
    popUp->foreground_color = foreground_color;
    popUp->text_color = text_color;
}

void Knob::updatePopUp()
{
    popUp->background_color = background_color;
    popUp->foreground_color = foreground_color;
    popUp->text_color = text_color;
    // normalize val
    float val = (value_ - min) / (max - min);
    // multiply by real value
    val = fabs(real_min - real_max) * val + real_min;
    char val_str[36];
    sprintf(val_str, format_str, val);
    popUp->setText(val_str);
    popUp->resize();
    const int pop_x = getAbsoluteX() + getWidth() / 2 - popUp->getWidth() / 2;
    popUp->setAbsoluteX(pop_x);
}

END_NAMESPACE_DISTRHO