/* license */
#include "Menu.hpp"

START_NAMESPACE_DISTRHO

Menu::Menu(Window &parent) noexcept
    : NanoWidget(parent)
{
    parent.addIdleCallback(this);
    loadSharedResources();
    highlighted_item_ = -1;
    font_size = 14;
    margin = 2.f;
    timer = 0; // if >= 0 menu is visible
    time_out = 150;
}
Menu::Menu(Widget *widget) noexcept
    : NanoWidget(widget)
{
    widget->getParentWindow().addIdleCallback(this);
    loadSharedResources();
    highlighted_item_ = -1;
    font_size = 14;
    margin = 2.f;
    timer = 0; // if >= 0 menu is visible
    time_out = 150;
}

void Menu::idleCallback()
{
    if (!has_mouse_ && !dropdown_has_mouse && timer > -1)
    {
        timer--;
    }

    if (timer < 0)
        hide();
}

bool Menu::onMouse(const MouseEvent &ev)
{
    if (ev.press && ev.button == 1)
    {
        if (contains(ev.pos))
        {
            if (highlighted_item_ >= 0)
            {
                callback_->onMenuClicked(this, highlighted_item_, items_[highlighted_item_]);
                return true;
            }
        }
        else
        {
            hide();
            return true;
        }
    }
    return false;
}

bool Menu::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos))
    {
        has_mouse_ = true;
        timer = time_out;
        const int mouse_y = ev.pos.getY();
        highlighted_item_ = mouse_y / font_size;
        repaint();
    }
    else
    {
        has_mouse_ = false;
        if (highlighted_item_ >= 0)
        {
            highlighted_item_ = -1;
            repaint();
        }

        if (timer < 0 && isVisible())
        {
            hide();
            repaint();
        }
    }

    return false;
}

void Menu::onNanoDisplay()
{
    int width = getWidth();
    int height = getHeight();
    beginPath();
    fillColor(background_color);
    rect(0, 0, width, height);
    fill();
    closePath();
    fontSize(font_size);
    if (highlighted_item_ >= 0)
    {
        beginPath();
        rect(0, font_size * highlighted_item_, width, font_size);
        fillColor(highlight_color);
        fill();
        closePath();
        ;
    }
    int j = 0;
    textAlign(ALIGN_TOP);
    for (auto i : items_)
    {
        if (j == highlighted_item_)
        {
            fillColor(background_color);
        }
        else
        {
            fillColor(text_color);
        }
        text(0, font_size * j + margin, i.c_str(), nullptr);
        j++;
    }
}

void Menu::setCallback(Callback *cb)
{
    callback_ = cb;
}

void Menu::addItem(std::string item)
{
    items_.push_back(item);
    resize();
}

void Menu::resize()
{
    float bounds[4];
    std::stringstream buffer;
    for (auto i : items_)
    {
        buffer << i << "\n";
    }
    textAlign(ALIGN_LEFT | ALIGN_TOP);
    textBoxBounds(0.f, 0.f, 200.f, buffer.str().c_str(), nullptr, bounds);
    setSize(bounds[2] - bounds[0], bounds[3] - bounds[1]);
}

END_NAMESPACE_DISTRHO
