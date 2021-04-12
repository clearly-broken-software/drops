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
#include "HBox.hpp"

START_NAMESPACE_DISTRHO

HBox::HBox(Window &parent) noexcept
    : NanoWidget(parent),
      align_items(Align_Items::middle),
      justify_content(Justify_Content::space_evenly),
      padding(0)
{
}

HBox::HBox(Widget *widget) noexcept
    : NanoWidget(widget),
      align_items(Align_Items::middle),
      justify_content(Justify_Content::space_evenly),
      padding(0)
{
}

void HBox::addWidget(Widget *widget)
{
    items_.emplace_back(Item(widget));

    const uint box_height = getHeight();
    const uint wh = widget->getHeight();
    if (wh > box_height)
        setHeight(wh);

    for (auto it = items_.begin(); it != items_.end(); it++)
        it->height = getHeight();
}

void HBox::setWidgetAlignment(uint id, Align_Items a_i)
{
    for (auto it = items_.begin(); it != items_.end(); it++)
    {
        if (it->widget->getId() == id)
        {
            it->align_self = a_i;
            positionWidgets();
            return;
        }
    }
}
void HBox::setWidgetJustify_Content(uint id, Justify_Content j_c)
{
    for (auto it = items_.begin(); it != items_.end(); it++)
    {
        if (it->widget->getId() == id)
        {
            it->justify_content = j_c;
            positionWidgets();
            return;
        }
    }
}

void HBox::removeWidget(uint id)
{
    for (auto it = items_.begin(); it != items_.end(); it++)
    {
        if (it->widget->getId() == id)
        {
            items_.erase(it);
            positionWidgets();
            return;
        }
    }
}

void HBox::positionWidgets()
{
    const uint width = getWidth();
    const uint height = getHeight();
    const uint box_x = getAbsoluteX();
    const uint box_y = getAbsoluteY();

    switch (justify_content)
    {
    case Justify_Content::left:
    {
        uint step = 0;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->x = box_x + step;
            it->widget->setAbsoluteX(it->x);
            it->widget->setAbsoluteY(box_y);
            const uint ww = it->widget->getWidth();
            step += ww;
            step += padding;
            it->width = ww;
        }
        break;
    }
    case Justify_Content::right:
    {
        uint combined_widget_width = 0;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            combined_widget_width += it->widget->getWidth();
        }
        uint startX = box_x + width - combined_widget_width;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->widget->setAbsoluteX(startX);
            it->widget->setAbsoluteY(box_y);
            it->x = startX;
            const uint ww = it->widget->getWidth();
            startX += ww;
            it->width = ww;
        }

        break;
    }
    case Justify_Content::center:
    {
        uint combined_widget_width = 0;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            combined_widget_width += it->widget->getWidth();
        }

        int startX = box_x + width / 2 - combined_widget_width / 2;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->widget->setAbsoluteX(startX);
            it->widget->setAbsoluteY(box_y);
            it->x = startX;
            const uint ww = it->widget->getWidth();
            startX += ww;
            it->width = ww;
        }
        break;
    }
    case Justify_Content::space_evenly:
    {
        uint number_of_items = items_.size();
        uint item_width = width / number_of_items;
        uint step = 0;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            switch (it->justify_content)
            {
            case Justify_Content::left:
                it->widget->setAbsoluteX(box_x + step);
                break;
            case Justify_Content::right:
                it->widget->setAbsoluteX(box_x + step + item_width - it->widget->getWidth());
                break;
            case Justify_Content::center:
            case Justify_Content::space_evenly:
            case Justify_Content::none:
            default:
                const uint ww = it->widget->getWidth();
                it->widget->setAbsoluteX(box_x + step + (item_width / 2 - ww / 2));
                break;
            }
            it->x = box_x + step;
            it->widget->setAbsoluteY(box_y);
            it->width = item_width;
            step += item_width;
        }
    }
    default:
        break;
    }

    switch (align_items)
    {
    case Align_Items::top:
    {
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->widget->setAbsoluteY(box_y);
        }
        break;
    }
    case Align_Items::middle:
    {
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->widget->setAbsoluteY(box_y + height / 2 - it->widget->getHeight() / 2);
        }
        break;
    }
    case Align_Items::bottom:
    {
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->widget->setAbsoluteY(box_y + height - it->widget->getHeight());
        }
        break;
    }
    default:
        break;
    }

    for (auto it = items_.begin(); it != items_.end(); it++)
    {
        switch (it->align_self)
        {
        case Align_Items::top:
            it->widget->setAbsoluteY(box_y);
            break;
        case Align_Items::middle:
            it->widget->setAbsoluteY(box_y + height / 2 - it->widget->getHeight() / 2);
            break;
        case Align_Items::bottom:
            it->widget->setAbsoluteY(box_y + height - it->height);
            break;
        case Align_Items::none:
        default:
            break;
        }
    }

    for (auto it = items_.begin(); it != items_.end(); it++)
    {
        uint item_x = it->x;
        uint item_w = it->width;
        switch (it->justify_content)
        {
        case Justify_Content::left:
            it->widget->setAbsoluteX(item_x);
            break;
        case Justify_Content::right:
            it->widget->setAbsoluteX(item_x + item_w - it->widget->getWidth());
            /* code */
            break;
        // case Justify_Content::center:
        // case Justify_Content::space_evenly:
        // case Justify_Content::none:
        default:

            break;
        }
    }
}

void HBox::onNanoDisplay()
{
// #ifdef DEBUG
//     const uint width = getWidth();
//     const uint height = getHeight();
//     const uint box_x = getAbsoluteX();
//     const uint box_y = getAbsoluteY();
//     const float stroke_width = 1.0f;
//     const float dbl_stroke = stroke_width * 2.f;
//     fillColor(1.0f, 1.0f, 1.0f, 0.1f);
//     strokeColor(1.0f, 0.f, 0.f, .5f);
//     strokeWidth(stroke_width);

//     for (auto it = items_.begin(); it != items_.end(); it++)
//     {

//         beginPath();
//         const uint x = it->x - box_x;
//         const uint y = 0;
//         rect(x, y, it->width, it->height);
//         fill();
//         stroke();
//         closePath();
//     }
// #endif
}

END_NAMESPACE_DISTRHO
