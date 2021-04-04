/*  license  */

#ifndef KNOB_HPP
#define KNOB_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include "PopUp.hpp"
#include <iostream>
#include <string>

START_NAMESPACE_DISTRHO

class Knob : public NanoWidget,
             public IdleCallback
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void knobDragStarted(Knob *knob) = 0;
        virtual void knobDragFinished(Knob *knob, float value) = 0;
        virtual void knobValueChanged(Knob *knob, float value) = 0;
    };
    explicit Knob(Window &parent) noexcept;
    explicit Knob(Widget *widget) noexcept;
    void setCallback(Callback *cb);
    void setValue(float val, bool sendCallback = false) noexcept;
    float getValue() noexcept;
    void setPopUp(PopUp *popUp);
    void idleCallback() override;
    void setStepText(std::initializer_list<const char*> strings);
     void setFont(const char *name, const uchar *data, uint dataSize);

    std::string label; // public, no getter or setter
    std::string unit;  // Hz, dB, Ct
    float labelSize;
    float gauge_width;
    Color background_color;
    Color foreground_color;
    Color highlight_color;
    Color text_color;
    float margin;
    float default_value;
    float step_value;
    float min;
    float max;
    float real_min;
    float real_max;
    const char *format_str; // also include unit !
    bool using_log;
    bool is_centered;

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;
    bool onScroll(const ScrollEvent &) override;

private:
    Callback *callback;
    PopUp *popUp;
    std::vector<const char*>stepText;
    bool dragging_;
    float value_;
    float value_tmp_;
    int mouseY_;
    FontId font_;
    Color fill_color_;
    bool has_mouse_;

    float tmp_value_;
    bool using_default_;

    int last_mouse_x_;
    int last_mouse_y_;
    bool is_ready_;
    float _logscale(float value) const;
    float _invlogscale(float value) const;
    void updatePopUp();

    int countdown_;
    bool is_counting_down_;

    DISTRHO_LEAK_DETECTOR(Knob)
};

END_NAMESPACE_DISTRHO

#endif // SCROLLBAR_HPP
