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
