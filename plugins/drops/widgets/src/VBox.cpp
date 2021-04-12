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
#include "VBox.hpp"

START_NAMESPACE_DISTRHO

VBox::VBox(Window &parent) noexcept
    : NanoWidget(parent),
      align_items(Align_Items::space_evenly),
      justify_content(Justify_Content::center)
{
}
VBox::VBox(Widget *widget) noexcept
    : NanoWidget(widget),
      align_items(Align_Items::space_evenly),
      justify_content(Justify_Content::center)
{
}

void VBox::addWidget(Widget *widget)
{
    items_.emplace_back(Item(widget));

    uint box_width = getWidth();
    uint ww = widget->getWidth();
    if (ww > box_width)
        setWidth(ww);

    // positionWidgets();
}

void VBox::setWidgetAlignment(uint id, Align_Items a_i)
{
    for (auto it = items_.begin(); it != items_.end(); it++)
    {
        if (it->widget->getId() == id)
        {
            it->align_self = a_i;
            return;
        }
    }
}
void VBox::setWidgetJustify_Content(uint id, Justify_Content j_c)
{
    for (auto it = items_.begin(); it != items_.end(); it++)
    {
        if (it->widget->getId() == id)
        {
            it->justify_content = j_c;
            return;
        }
    }
}

void VBox::setWidgetResize(uint id, bool ) // FIXME: do something about the bool resize
{
    for (auto it = items_.begin(); it != items_.end(); it++)
    {
        if (it->widget->getId() == id)
        {
            it->can_resize = true;
            return;
        }
    }
}

void VBox::removeWidget(uint id)
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

void VBox::positionWidgets()
{
    const uint width = getWidth();
    const uint height = getHeight();
    const uint box_x = getAbsoluteX();
    const uint box_y = getAbsoluteY();

    switch (justify_content)
    {
    case Justify_Content::left:
    {
        for (auto it = items_.begin(); it != items_.end(); it++)
        {

            it->widget->setAbsoluteX(box_x);
            const uint ww = it->widget->getWidth();
            it->width = ww;
            it->x = box_x;
        }
        break;
    }
    case Justify_Content::right:
    {
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->widget->setAbsoluteX(box_x + width - it->widget->getWidth());
            it->x = box_x;
            it->width = width;
        }

        break;
    }
    case Justify_Content::center:
    {
        uint thisX = getAbsoluteX();
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            const uint half_width = it->widget->getWidth() / 2;
            const uint center = width / 2;
            it->widget->setAbsoluteX(thisX + center - half_width);
            it->x = box_x;
            it->width = width;
        }
        break;
    }
    default:
        break;
    }

    switch (align_items)
    {
    case Align_Items::top:
    {
        uint step = 0;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->widget->setAbsoluteY(box_y + step);
            it->y = box_y + step;
            const uint wh = it->widget->getHeight();
            step += wh;
            it->height = wh;
        }
        break;
    }
    case Align_Items::middle:
    {
        uint combined_height = 0;
        for (auto it = items_.begin(); it != items_.end(); it++)
            combined_height += it->widget->getHeight();

        uint startY = box_y + height / 2 - combined_height / 2;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->widget->setAbsoluteY(startY);
            it->y = startY;
            uint wh = it->widget->getHeight();
            it->height = wh;
            startY += wh;
        }
        break;
    }
    case Align_Items::bottom:
    {
        uint combined_height = 0;
        for (auto it = items_.begin(); it != items_.end(); it++)
            combined_height += it->widget->getHeight();

        uint startY = box_y + height - combined_height;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            it->widget->setAbsoluteY(startY);
            uint wh = it->widget->getHeight();
            it->height = wh;
            startY += wh;
        }
        break;
    }

    case Align_Items::space_evenly:
    {
        uint number_of_items = items_.size();
        uint item_height = height / number_of_items;
        uint step = 0;
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            switch (it->align_self)
            {
            case Align_Items::top:
                it->widget->setAbsoluteY(box_y + step);
                break;
            case Align_Items::bottom:
                it->widget->setAbsoluteY(box_y + step + item_height - it->widget->getHeight());
                break;
            case Align_Items::middle:
            case Align_Items::space_evenly:
            case Align_Items::none:
            default:
                if (it->can_resize)
                {
                    it->height = item_height;
                    it->widget->setHeight(item_height);
                    it->widget->setAbsoluteY(box_y + step);
                }
                else
                {
                    uint wh = it->widget->getHeight();
                    it->widget->setAbsoluteY(box_y + step + (item_height / 2 - wh / 2));
                }

                break;
            }
            it->y = box_y + step;
            step += item_height;
            it->height = item_height;
        }
    }
    default:
        break;
    }
    /* for (auto it = items_.begin(); it != items_.end(); it++)
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
    } */
}

void VBox::onNanoDisplay()
{

// #ifdef DEBUG
//     const uint width = getWidth();
//     const uint height = getHeight();
//     const uint box_x = getAbsoluteX();
//     const uint box_y = getAbsoluteY();
//     const float stroke_width = 1.0f;
//     const float dbl_stroke = stroke_width * 2.f;
//     const uint id = getId();
//     fillColor(0.0f, 1.0f, 1.0f, 0.1f);
//     strokeColor(1.0f, 1.f, 0.f, .5f);
//     strokeWidth(stroke_width);
//     uint step = 0;

//     for (auto it = items_.begin(); it != items_.end(); it++)
//     {

//         beginPath();
//         const uint x = it->x - box_x;
//         const uint y = it->y - box_y;
//         rect(x, y, it->width, it->height);
//         fill();
//         stroke();
//         closePath();
//     }
// #endif
}

END_NAMESPACE_DISTRHO
