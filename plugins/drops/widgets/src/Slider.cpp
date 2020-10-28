/* license */
#include "Slider.hpp"

START_NAMESPACE_DISTRHO

Slider::Slider(Window &parent) noexcept
    : NanoWidget(parent)
{
    loadSharedResources();
    dragging_ = false;
    has_mouse_ = false;
    value_ = 0.f;
    value_tmp_ = 0.f;
    max_value = 1.0f;
    min_value = 0.0f;
    using_log_ = false;
    font_size = 14.0f;
    fontFace(NANOVG_DEJAVU_SANS_TTF);
    font_ = findFont(NANOVG_DEJAVU_SANS_TTF);
    if (font_ == -1)
    {
        fprintf(stderr, "%s", "font not found\n");
    }

    setLabel("LABEL:"); /// also sets handle size

    margin_ = handle_.getWidth() / 2.0f;
    foreground_color = Color(1, 1, 1);
    background_color = Color(0, 0, 0);
    text_color = Color(1, 1, 1);
    fill_color_ = foreground_color;
}

Slider::Slider(Widget* widget) noexcept
    : NanoWidget(widget)
{
    loadSharedResources();
    dragging_ = false;
    has_mouse_ = false;
    value_ = 0.f;
    value_tmp_ = 0.f;
    max_value = 1.0f;
    min_value = 0.0f;
    using_log_ = false;
    font_size = 14.0f;
    fontFace(NANOVG_DEJAVU_SANS_TTF);
    font_ = findFont(NANOVG_DEJAVU_SANS_TTF);
    if (font_ == -1)
    {
        fprintf(stderr, "%s", "font not found\n");
    }

    setLabel("LABEL:"); /// also sets handle size

    margin_ = handle_.getWidth() / 2.0f;
    foreground_color = Color(1, 1, 1);
    background_color = Color(0, 0, 0);
    text_color = Color(1, 1, 1);
    fill_color_ = foreground_color;
}



void Slider::setLabel(std::string l)
{
    label_ = l;
    fontFaceId(font_);
    fontSize(font_size);
    Rectangle<float> bounds;
    textBounds(0.f, 0.f, label_.c_str(), NULL, bounds);
    label_width_ = bounds.getWidth();
    label_height_ = bounds.getHeight();
    handle_.setSize(font_size, font_size);
    handle_.setPos(label_width_ + margin_, getHeight() - handle_.getHeight());
}

float Slider::getValue() noexcept
{
    return value_;
}
bool Slider::onMouse(const MouseEvent &ev)
{

    bool handle_has_mouse = handle_.contains(ev.pos);
    if (ev.press == 1 && handle_has_mouse)
    {
        has_mouse_ = true;
        dragging_ = true;
        last_mouse_x_ = ev.pos.getX();
        return false;
    }
    else if (dragging_)
    {
        has_mouse_ = false;
        dragging_ = false;
        return false;
    }
    return false;
}

bool Slider::onScroll(const ScrollEvent &ev)
{
    if (!contains(ev.pos))
        return false;

    const float d = (ev.mod & kModifierControl) ? 2000.0f : 200.0f;
    float value = (using_log_ ? _invlogscale(tmp_value_) : tmp_value_) + (float(max_value - min_value) / d * 10.f * ev.delta.getY());

    if (using_log_)
        value = _logscale(value);

    if (value < min_value)
    {
        tmp_value_ = value = min_value;
    }
    else if (value > max_value)
    {
        tmp_value_ = value = max_value;
    }
    setValue(value);
    repaint();
    return true;
}

bool Slider::onMotion(const MotionEvent &ev)
{
    if (handle_.contains(ev.pos) && !has_mouse_)
    {
        has_mouse_ = true;
        repaint();
    }
    if (!handle_.contains(ev.pos) && !dragging_ && has_mouse_)
    {
        has_mouse_ = false;
        repaint();
    }
    if (!dragging_)
        return false;
    const float mx = static_cast<float>(ev.pos.getX());
    const float slider_area_w = static_cast<float>(getWidth() - label_width_ - 2 * margin_);
    const float vper = (mx - label_width_) / slider_area_w;
    const float val = vper * (max_value - min_value);
    setValue(val);
    printf("slider onMotion repaint\n");
    repaint();
    return true;
}

void Slider::onNanoDisplay()
{
    const float height = getHeight();
    const float width = getWidth();
    const float stroke_width = 2.f; // FIXME: hard coded
    /* beginPath();
    fillColor(1.0f, 1.0f, 1.0f, 0.1f);
    rect(0, 0, width, height);
    fill();
    closePath();
     */

    // label
    const float label_x = 0.0f;
    const float label_y = height - label_height_;
    beginPath();
    fillColor(text_color);
    fontSize(font_size);
    textAlign(ALIGN_TOP | ALIGN_LEFT);
    text(label_x, label_y, label_.c_str(), NULL);
    closePath();

    //Line
    const float line_x = label_width_ + margin_;
    const float line_y = height - stroke_width / 2.f;
    const float line_end_x = width - margin_;
    beginPath();
    strokeColor(background_color);
    strokeWidth(stroke_width);
    moveTo(line_x, line_y);
    lineTo(line_end_x, line_y);
    stroke();
    closePath();

    //Handle (value)

    if (has_mouse_)
    {
        fill_color_ = highlight_color;
    }
    else
    {
        fill_color_ = foreground_color;
    }
    float normalized_X = line_x + (line_end_x - line_x) * value_;
    const float hw = handle_.getWidth();
    const float hh = handle_.getHeight();
    handle_.setX(normalized_X - hw / 2.f);
    const float hx = handle_.getX();
    const float hy = handle_.getY();
    fillColor(fill_color_);
    beginPath();
    moveTo(hx, height - hh);
    lineTo(hx + hw, height - hh);
    lineTo(hx + hw / 2, height);
    lineTo(hx, height - hh);
    fill();
    closePath();
}

void Slider::setValue(float val) noexcept
{
    value_ = std::max(min_value, std::min(val, max_value));
    tmp_value_ = value_;
    callback->onSliderValueChanged(this, value_);
}

float Slider::_logscale(float value) const
{
    const float b = std::log(max_value / min_value) / (max_value - min_value);
    const float a = max_value / std::exp(max_value * b);
    return a * std::exp(b * value);
}

float Slider::_invlogscale(float value) const
{
    const float b = std::log(max_value / min_value) / (max_value - min_value);
    const float a = max_value / std::exp(max_value * b);
    return std::log(value / a) / b;
}

void Slider::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
