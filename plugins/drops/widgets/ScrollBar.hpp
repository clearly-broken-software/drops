/*  license  */

#ifndef SCROLLBAR_HPP
#define SCROLLBAR_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class ScrollBar : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void onScrollBarClicked(ScrollBar *scrollBar, bool dragging) = 0;
    };
    explicit ScrollBar(Window &parent) noexcept;
    void setCallback(Callback *cb);

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;

private:
    Callback *callback;
    bool dragging;

    DISTRHO_LEAK_DETECTOR(ScrollBar)
};

END_NAMESPACE_DISTRHO

#endif // SCROLLBAR_HPP
