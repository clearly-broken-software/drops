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
#include "ScrollBar.hpp"

START_NAMESPACE_DISTRHO

ScrollBar::ScrollBar(Window &parent) noexcept
    : NanoWidget(parent)
{
    dragging = false;
}

bool ScrollBar::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        dragging = true;
        callback->onScrollBarClicked(this, dragging);
        return false;
    }
    else if (!ev.press && ev.button == 1 && dragging)
    {
        dragging = false;
        callback->onScrollBarClicked(this, dragging);
        return false;
    }
    else
    {
        return false;
    }
}

void ScrollBar::onNanoDisplay()
{
    // #ifdef DEBUG   
    // int w = getWidth();
    // int h = getHeight();
    // beginPath();
    // strokeColor(255, 0, 0);
    // strokeWidth(1.0f);
    // rect(1, 1, w-2, h-2);
    // stroke();
    // closePath();
    // #endif
    
}

void ScrollBar::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
