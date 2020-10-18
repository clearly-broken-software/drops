/*  license  */

#ifndef KNOB_HPP
#define KNOB_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include <iostream>
#include <string>

START_NAMESPACE_DISTRHO

class Knob : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void knobValueChanged(Knob *knob, float value) = 0;
    };
    explicit Knob(Window &parent) noexcept;
    void setCallback(Callback *cb);
    void setValue(float val) noexcept;
    std::string label; // public, no getter or setter
    float labelSize;
    Color background_color;
    Color foreground_color;
    Color highlite_color;
    Color text_color;
    float margin;
    
protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;

private:
    Callback *callback;
    bool dragging_;
    float value_;
    float valueTmp_;
    int mouseY_;
    FontId font_;
    Color fill_color_;
    bool has_mouse_;
    

    DISTRHO_LEAK_DETECTOR(Knob)
};

END_NAMESPACE_DISTRHO

#endif // SCROLLBAR_HPP
