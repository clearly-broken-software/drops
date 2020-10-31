/*  license  */

#ifndef H_BOX_HPP
#define H_BOX_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include <vector>

START_NAMESPACE_DISTRHO

class HBox : public NanoWidget
{
public:
    explicit HBox(Window &parent) noexcept;
    explicit HBox(Widget *widget) noexcept;
    enum struct Align_Items
    {
        none,
        top,
        middle,
        bottom
    };

    enum struct Justify_Content
    {
        none,
        left,
        right,
        center,
        space_evenly
    };

    Align_Items align_items;
    Justify_Content justify_content;

    void addWidget(Widget *widget);
    void setWidgetAlignment(uint id, Align_Items align_self);
    void setWidgetJustify_Content(uint id, Justify_Content justify_content);
    void removeWidget(uint id);
    Color background_color;
    Color foreground_color;
    Color highlight_color;
    Color text_color;
    void positionWidgets();
    int padding;

protected:
    void onNanoDisplay() override;

private:
    struct Item
    {
        Item(Widget *w)
        {
            widget = w;
            width = 0;
            height = 0;
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

    Widget *parent_ = nullptr;

    std::vector<Item> items_;

    DISTRHO_LEAK_DETECTOR(HBox)
};

END_NAMESPACE_DISTRHO

#endif // SCROLLBAR_HPP
