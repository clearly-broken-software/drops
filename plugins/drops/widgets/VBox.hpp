/*  license  */

#ifndef V_BOX_HPP
#define V_BOX_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include <vector>

START_NAMESPACE_DISTRHO

class VBox : public NanoWidget
{
public:
    explicit VBox(Window &parent) noexcept;
    explicit VBox(Widget *widget) noexcept;
    enum struct Align_Items
    {
        none,
        top,
        middle,
        bottom,
        space_evenly
    };

    enum struct Justify_Content
    {
        none,
        left,
        right,
        center
    };

    Align_Items align_items;
    Justify_Content justify_content;
    
    void addWidget(Widget *widget);
    void setWidgetAlignment(uint id,Align_Items align_self);
    void setWidgetJustify_Content(uint id,Justify_Content justify_content);
    void removeWidget(uint id);
    Color background_color;
    Color foreground_color;
    Color highlite_color;
    Color text_color;
    int padding;

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;
    bool onScroll(const ScrollEvent &) override;

private:

    struct Item
    {
        Item( Widget *w)
        {
            widget = w;
            width = 0;
            height = 0;
            justify_content = Justify_Content::none; 
            align_self = Align_Items::none;           
        }

        uint width;
        uint height;
        uint x;
        uint y;
        Widget *widget;
        Align_Items align_self;
        Justify_Content justify_content;
    };

    void positionWidgets();
    std::vector<Item> items_;

    DISTRHO_LEAK_DETECTOR(VBox)
};

END_NAMESPACE_DISTRHO

#endif // SCROLLBAR_HPP
