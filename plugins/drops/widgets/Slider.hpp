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

#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include <iostream>
#include <string>

START_NAMESPACE_DISTRHO

class Slider : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        //    virtual void sliderDragStarted(Slider *slider) = 0;
        //   virtual void sliderDragFinished(Slider *slider) = 0;
        virtual void onSliderValueChanged(Slider *slider, float value) = 0;
    };
    explicit Slider(Window &parent) noexcept;
    explicit Slider(Widget *widget) noexcept;
    void setCallback(Callback *cb);
    void setValue(float val) noexcept;
    float getValue() noexcept;

    void setLabel(std::string label);
    void setSliderArea();
    float font_size;
    Color background_color;
    Color foreground_color;
    Color highlight_color;
    Color text_color;
    const char *unit;
    const char *format_str;
    // space for value and unit
    float right_padding;
    float default_value;
    float min_value;
    float max_value;

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;
    bool onScroll(const ScrollEvent &) override;

private:
   
    Callback *callback;
    Rectangle<int> handle_;
    std::string label_;
    float margin_;
    float slider_length_;
    bool dragging_;
    float value_;
    float value_tmp_;
    int mouseX_;
    FontId font_;
    Color fill_color_;
    bool has_mouse_;
    float tmp_value_;
    bool using_default_;
    bool using_log_;
    int last_mouse_x_;
    int last_mouse_y_;
    bool is_ready_;
    float label_width_;
    float label_height_;
    float _logscale(float value) const;
    float _invlogscale(float value) const;

    DISTRHO_LEAK_DETECTOR(Slider)
};

END_NAMESPACE_DISTRHO

#endif // SCROLLBAR_HPP
