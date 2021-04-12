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
#include "Menu.hpp"

START_NAMESPACE_DISTRHO

Menu::Menu(Window &parent) noexcept
    : NanoWidget(parent)
{
    parent.addIdleCallback(this);
    highlighted_item_ = -1;
    font_size = 14;
    margin = 2.f;
    timer = 0; // if >= 0 menu is visible
    time_out = 150;
    max_view_items = 4;
    item_view_first = 0;
    item_view_last = max_view_items - 1;
    has_mouse_ = false;
    dropdown_has_mouse = false;
    menu_font_ = 0;
}
Menu::Menu(Widget *widget) noexcept
    : NanoWidget(widget)
{
    widget->getParentWindow().addIdleCallback(this);
    highlighted_item_ = -1;
    font_size = 14;
    margin = 2.f;
    timer = 0; // if >= 0 menu is visible
    time_out = 150;
    max_view_items = 4;
    item_view_first = 0;
    item_view_last = max_view_items - 1;
    has_mouse_ = false;
    dropdown_has_mouse = false;
    menu_font_ = 0;
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
    if (!isVisible())
        return false;
    if (ev.press && ev.button == 1)
    {
        if (contains(ev.pos))
        {
            if (highlighted_item_ >= 0)
            {
                const uint index = highlighted_item_ + item_view_first;
                callback_->onMenuClicked(this, index, items_[index]);
                return true;
            }
            if (ev.pos.getY() < font_size)
            {
                scrollMenu(-1); // scroll back
                return false;
            }
            if (ev.pos.getY() > getHeight() - font_size)
            {
                scrollMenu(1);
                return false;
            }
        }
        else
        {
            hide();
            return false;
        }
    }
    return false;
}

bool Menu::onMotion(const MotionEvent &ev)
{
    if (!isVisible())
        return false;
    if (contains(ev.pos))
    {
        int offset_y = max_size_reached_ ? 1 : 0;
        has_mouse_ = true;
        timer = time_out;
        const int mouse_y = ev.pos.getY();

        highlighted_item_ = mouse_y / font_size;
        highlighted_item_ -= offset_y;
        if (highlighted_item_ > item_view_last)
        {
            highlighted_item_ = -1;
        }
        repaint();
        return true;
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
        return false;
    }

    return true;
}

bool Menu::onScroll(const ScrollEvent &ev)
{
    if (!isVisible())
        return false;
    if (!contains(ev.pos))
        return false;
    if (max_size_reached_)
    {
        const int delta = -static_cast<int>(ev.delta.getY());
        scrollMenu(delta);
    }
    return true;
}

void Menu::onNanoDisplay()
{
    int width = getWidth();
    int height = getHeight();

    if(getAbsoluteY()+height>static_cast<int>(UI_H))
    {
        setAbsoluteY(UI_H - height - margin);
    }
    
    fontFaceId(menu_font_);
    beginPath();
    fillColor(background_color);
    rect(0, 0, width, height);
    fill();
    stroke();
    closePath();
    float y_offset = 0;
    // up/down arrows
    if (max_size_reached_)
    {
        y_offset = font_size;

        fillColor(foreground_color);
        beginPath();
        rect(0, 0, width, font_size);
        fill();
        closePath();
        beginPath();
        rect(0, height - font_size, width, font_size);
        fill();
        closePath();
        // triangles
        const float center_x = static_cast<float>(width) / 2.0f;
        beginPath();
        fillColor(text_color);
        moveTo(center_x, 0);
        lineTo(center_x + font_size / 2.0f, font_size);
        lineTo(center_x - font_size / 2.0f, font_size);
        lineTo(center_x, 0);
        fill();
        closePath();

        beginPath();
        fillColor(text_color);
        moveTo(center_x, height);
        lineTo(center_x + font_size / 2.0f, height - font_size);
        lineTo(center_x - font_size / 2.0f, height - font_size);
        lineTo(center_x, height);
        fill();
        closePath();
    }

    fontSize(font_size);
    if (highlighted_item_ >= 0)
    {
        beginPath();
        rect(0, font_size * highlighted_item_ + y_offset, width, font_size);
        fillColor(highlight_color);
        fill();
        closePath();
    }
    int j = 0;
    textAlign(ALIGN_TOP);
    int max_i = std::min(static_cast<int>(items_.size()), max_view_items);
    for (int i = 0; i < max_i; i++)
    {
        beginPath();
        j = i + item_view_first;
        if (j == highlighted_item_ + item_view_first)
        {
            fillColor(background_color);
        }
        else
        {
            fillColor(text_color);
        }
        text(0, font_size * i + margin + y_offset, items_[j], nullptr);
        closePath();
    }
}

void Menu::setCallback(Callback *cb)
{
    callback_ = cb;
}

void Menu::addItems(std::initializer_list<const char *> item_list)
{
    items_.insert(items_.end(), item_list.begin(), item_list.end());
    /*     for (auto elem : item_list)
    {
        items_.push_back(elem);
        max_size_reached_ = items_.size() > max_view_items ? true : false;
    } */
    max_size_reached_ = static_cast<int>(items_.size()) > max_view_items ? true : false;
    resize();
}

void Menu::setMaxViewItems(int maxViewItems)
{
    max_view_items = maxViewItems;
    item_view_last = max_view_items -1;
}

void Menu::resize()
{

    // measure total size
    float bounds[4]; // xmin,ymin xmax,ymax
    std::stringstream buffer;
    for (auto i : items_)
    {
        buffer << i << "\n";
    }
    beginPath();
    fontSize(font_size);
    textAlign(ALIGN_LEFT | ALIGN_TOP);
    textBoxBounds(0.f, 0.f, 200.f, buffer.str().c_str(), nullptr, bounds);
    closePath();

    const float width = bounds[2] - bounds[0];
    float height = bounds[3] - bounds[1];

    if (max_size_reached_) // add up and down 'arrow'
        height = max_view_items * (font_size) + (2 * font_size);

    setSize(width, height);
}

void Menu::scrollMenu(int dir)
{
    item_view_first += dir;
    const int upper = items_.size() - max_view_items;
    const int lower = 0;
    item_view_first = std::min(upper, std::max(item_view_first, lower));
    repaint();
}

std::string Menu::getItem(uint index)
{
    return items_[index];
}

void Menu::setFont(const char *fontName, const uchar *data, uint dataSize)
{
    menu_font_ = createFontFromMemory(fontName, data, dataSize, false);
}

END_NAMESPACE_DISTRHO
