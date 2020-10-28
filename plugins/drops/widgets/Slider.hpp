/*  license  */

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
    explicit Slider(Widget* widget) noexcept;
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
