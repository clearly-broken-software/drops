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
#include "DropDown.hpp"

START_NAMESPACE_DISTRHO

DropDown::DropDown(Window &parent) noexcept
    : NanoWidget(parent)
{
    //loadSharedResources();
    label = "Dropdown: ";
    item = "item";
    font_size = 14.f;
    margin = 2.f;
    parent.addIdleCallback(this);
    has_mouse_ = false;
    background_color = Color(0.0f, 0.0f, 0.0f);
    foreground_color = Color(0.5f, 0.5f, 0.5f);
    highlight_color = Color(0.3f, 0.3f, 0.3f);
    text_color = Color(1.0f, 1.0f, 1.0f);
    main_font_ = 0;
}
DropDown::DropDown(Widget *widget) noexcept
    : NanoWidget(widget)
{
    //loadSharedResources();
    label = "Dropdown: ";
    item = "item";
    font_size = 14.f;
    margin = 2.f;
    widget->getParentWindow().addIdleCallback(this);
    has_mouse_ = false;
    background_color = Color(0.0f, 0.0f, 0.0f);
    foreground_color = Color(0.5f, 0.5f, 0.5f);
    highlight_color = Color(0.3f, 0.3f, 0.3f);
    text_color = Color(1.0f, 1.0f, 1.0f);
    main_font_ = 0;
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
    Color fill_color(background_color);
    // background
    if (has_mouse_)
        fill_color = highlight_color;
    beginPath();
    fillColor(fill_color);
    rect(0, 0, width, height);
    fill();
    closePath();
    // label
    beginPath();
    fillColor(text_color);
    fontFaceId(main_font_);
    fontSize(font_size);
    textAlign(ALIGN_TOP);
    const float item_x_offset = getMenuOffset();
    text(0, margin, label.c_str(), nullptr);
    closePath();
    beginPath();
    fontFaceId(menu_font_);
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
    fontFaceId(main_font_);
    fontSize(font_size);
    textBounds(0, 0, label.c_str(), nullptr, bounds);
    return bounds.getX() + bounds.getWidth();
}

void DropDown::setMenu(Menu *menu)
{
    menu_ = menu;
}

void DropDown::positionMenu()
{
    if (menu_)
    {
        const int x = getAbsoluteX() + getMenuOffset();
        const int y = getAbsoluteY() + getHeight();
        menu_->setAbsolutePos(x, y);
    }
}

void DropDown::resize()
{
    float itemWidth = 0.f;
    if (menu_) // if menu is set, add width;
    {
        itemWidth = menu_->getWidth();
    }
    else // add width of item
    {
        Rectangle<float> bounds;
        textBounds(0, 0, item.c_str(), nullptr, bounds);
        itemWidth = bounds.getWidth();
    }
    setWidth(getMenuOffset() + itemWidth);
}

void DropDown::setFont(const char *fontName, const uchar *data, uint dataSize)
{
    main_font_ = createFontFromMemory(fontName, data, dataSize, false);
}

void DropDown::setMenuFont(const char *fontName, const uchar *data, uint dataSize)
{
    menu_font_ = createFontFromMemory(fontName, data, dataSize, false);
}

END_NAMESPACE_DISTRHO
