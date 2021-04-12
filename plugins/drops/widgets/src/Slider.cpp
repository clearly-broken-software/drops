/*
    Drops - Drops Really Only Plays Samples
    Copyright (C) 2021  Rob van den Berg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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

Slider::Slider(Widget *widget) noexcept
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
    unit = nullptr;
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
    handle_.setPos(label_width_ + margin_,
                   getHeight() / 2.0f - label_height_ / 2.0f);
    unit = nullptr;
}

float Slider::getValue() noexcept
{
    return value_;
}
bool Slider::onMouse(const MouseEvent &ev)
{
    if (!isVisible())
        return false;
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
        callback->onSliderValueChanged(this, value_);
        has_mouse_ = false;
        dragging_ = false;
        return false;
    }
    return false;
}

bool Slider::onScroll(const ScrollEvent &ev)
{
    if (!isVisible())
        return false;
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
    callback->onSliderValueChanged(this, value_);
    repaint();
    return true;
}

bool Slider::onMotion(const MotionEvent &ev)
{
    if (!isVisible())
        return false;
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
    const float slider_area_w = static_cast<float>(getWidth() - label_width_ - 2 * margin_ - right_padding);
    const float vper = (mx - label_width_) / slider_area_w;
    const float val = min_value + vper * (max_value - min_value);
    setValue(val);
    callback->onSliderValueChanged(this, value_);
    repaint();
    return true;
}

void Slider::onNanoDisplay()
{
    const float height = getHeight();
    const float width = getWidth();
    const float stroke_width = 2.f; // FIXME: hard coded

    // background
    beginPath();
    fillColor(background_color);
    rect(0, 0, width, height);
    fill();
    closePath();

    // label
    const float label_x = 0.0f;
    const float label_y = height / 2.0f - label_height_ / 2.0f;
    beginPath();
    fillColor(text_color);
    fontSize(font_size);
    textAlign(ALIGN_TOP | ALIGN_LEFT);
    text(label_x, label_y, label_.c_str(), NULL);
    closePath();

    //Line
    const float line_x = label_width_ + margin_;
    const float line_y = label_y + label_height_ + stroke_width / 2.f;
    const float line_end_x = width - margin_ - right_padding;
    beginPath();
    strokeColor(text_color);
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
    float normalized_val = (value_ - min_value) / (max_value - min_value);
    float normalized_X = line_x + (line_end_x - line_x) * normalized_val;
    const float hw = handle_.getWidth();
    handle_.setX(normalized_X - hw / 2.f);
    const float hx = handle_.getX();
    const float hy = handle_.getY();
    fillColor(fill_color_);
    beginPath();
    moveTo(hx, hy);
    lineTo(hx + hw, hy);
    lineTo(hx + hw / 2, line_y);
    lineTo(hx, hy);
    fill();
    closePath();

    // value
    const float val_x = width - right_padding;
    const float val_y = label_y;
    char val_str[10];
    sprintf(val_str, format_str, getValue());
    fillColor(text_color);
    beginPath();
    text(val_x, val_y, val_str, nullptr);
    closePath();

    //unit
    Rectangle<float> bounds;
    textBounds(0.0f, 0.0f, unit, nullptr, bounds);
    const float unit_w = bounds.getWidth();
    const float unit_x = width - unit_w;
    const float unit_y = label_y;

    beginPath();
    text(unit_x, unit_y, unit, nullptr);
    closePath();
}

void Slider::setValue(float val) noexcept
{
    value_ = std::max(min_value, std::min(val, max_value));
    tmp_value_ = value_;
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
