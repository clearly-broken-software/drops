/* license */
#include "DropDown.hpp"

START_NAMESPACE_DISTRHO

DropDown::DropDown(Window &parent) noexcept
    : NanoWidget(parent)
{
    loadSharedResources();
    label = "Dropdown: ";
    item = "item";
    font_size = 14.f;
    margin = 2.f;
    parent.addIdleCallback(this);
    has_mouse_ = false;
    background_color = Color(0.0f, 0.0f, 0.0f);
    foreground_color = Color(0.5f, 0.5f, 0.5f);
    text_color = Color(1.0f, 1.0f, 1.0f);
}
DropDown::DropDown(Widget *widget) noexcept
    : NanoWidget(widget)
{
    loadSharedResources();
    label = "Dropdown: ";
    item = "item";
    font_size = 14.f;
    margin = 2.f;
    widget->getParentWindow().addIdleCallback(this);
    has_mouse_ = false;
    background_color = Color(0.0f, 0.0f, 0.0f);
    foreground_color = Color(0.5f, 0.5f, 0.5f);
    text_color = Color(1.0f, 1.0f, 1.0f);
}

bool DropDown::onMouse(const MouseEvent &ev)
{
    if (!isVisible())
        return false;
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {

        callback_->onDropDownClicked(this);
        return false;
    }
    else
    {
        return false;
    }
}

bool DropDown::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos))
        has_mouse_ = true;
    else
        has_mouse_ = false;

    return false;
}

void DropDown::idleCallback()
{
    if (has_mouse_)
    {
        menu_->timer = menu_->time_out;
        menu_->dropdown_has_mouse = true;
    }
    else
    {
        menu_->dropdown_has_mouse = false;
    }
}

void DropDown::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();
    // background
    beginPath();
    fillColor(background_color);
    rect(0, 0, width, height);
    fill();
    closePath();
    // label
    beginPath();
    fillColor(text_color);
    fontSize(font_size);
    textAlign(ALIGN_TOP);
    // item
    const float item_x_offset = getMenuOffset();
    text(0, margin, label.c_str(), nullptr);
    text(item_x_offset, margin, item.c_str(), nullptr);
    closePath();
}

void DropDown::setCallback(Callback *cb)
{
    callback_ = cb;
}

void DropDown::setValue(float val)
{
    uint index = static_cast<uint>(val);
    item = menu_->getItem(index);
    repaint();
}


float DropDown::getMenuOffset()
{
    Rectangle<float> bounds;
    textBounds(0, 0, label.c_str(), nullptr, bounds);
    return bounds.getX() + bounds.getWidth();
}

void DropDown::setMenu(Menu *menu)
{
    menu_ = menu;
    // get coordinates
    int x = getAbsoluteX() + getMenuOffset();
    int y = getAbsoluteY() + getHeight();
    menu_->setAbsolutePos(x, y);
}

END_NAMESPACE_DISTRHO
