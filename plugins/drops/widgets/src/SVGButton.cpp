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
#include "SVGButton.hpp"

START_NAMESPACE_DISTRHO

SVGButton::SVGButton(Window &parent, char *svg, char *svgHL, float scale) noexcept
    : NanoWidget(parent)
{
    svgImage = new SVGImage(this, svg, scale);
    if (svgHL)
    {
        svgImageHL = new SVGImage(this, svgHL, scale);
    }
    else
    {
        svgImageHL = nullptr;
    }
    setSize(svgImage->getSize());
    has_mouse_ = false;
    is_active = false;
    background_color = Color(0.f, 0.f, 0.f);
    highlight_color = Color(1.f, 1.f, 1.f);
    active_color = Color(0.5f, 0.5f, 0.5f);
}

SVGButton::SVGButton(Widget *widget, char *svg, char *svgHL, float scale) noexcept
    : NanoWidget(widget)
{
    svgImage = new SVGImage(this, svg, scale);
    if (svgHL)
    {
        svgImageHL = new SVGImage(this, svgHL, scale);
    }
    else
    {
        svgImageHL = nullptr;
    }
    setSize(svgImage->getSize());
    has_mouse_ = false;
    is_active = false;
    background_color = Color(0.f, 0.f, 0.f);
    highlight_color = Color(1.f, 1.f, 1.f);
    active_color = Color(0.5f, 0.5f, 0.5f);
}

bool SVGButton::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        callback->onSVGButtonClicked(this);
        is_active = true;
        return true;
    }
    else
    {
        return false;
    }
}
bool SVGButton::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos) && !has_mouse_)
    {
        has_mouse_ = true;
        repaint();
    }
    if (!contains(ev.pos) && has_mouse_)
    {
        has_mouse_ = false;
        repaint();
    }
    return false;
}

void SVGButton::onNanoDisplay()
{
    const uint w = getWidth();
    const uint h = getHeight();
    beginPath();
    if (has_mouse_)
    {
        fill_color_ = highlight_color;
    }
    else if (is_active)
    {
        fill_color_ = active_color;
    }
    else
    {
        fill_color_ = background_color;
    }

    fillColor(fill_color_);
    rect(0, 0, w, h);
    fill();
    closePath();
    if (has_mouse_ && svgImageHL)
        svgImageHL->drawAt(0, 0);
    else
        svgImage->drawAt(0, 0);
}

void SVGButton::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
