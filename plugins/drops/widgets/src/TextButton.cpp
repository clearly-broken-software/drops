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
#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

TextButton::TextButton(Window &parent) noexcept
    : NanoWidget(parent)
{
    buttonText = "BUTTON";
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    loadSharedResources();
    has_mouse_ = false;
}

TextButton::TextButton(Widget *widget) noexcept
    : NanoWidget(widget)
{
    buttonText = "BUTTON";
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    loadSharedResources();
    has_mouse_ = false;
    isActive = false;
}

bool TextButton::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        callback->onTextButtonClicked(this);
        return true;
    }
    else
    {
        return false;
    }
}
bool TextButton::onMotion(const MotionEvent &ev)
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

void TextButton::onNanoDisplay()
{
    int width = getWidth();
    int height = getHeight();
    beginPath();
    fillColor(background_color);
    roundedRect(1, 1, width-2, height-2, 2);
    fill();
    closePath();
    
    beginPath();
    fillColor(background_color);
    rect(1, 5, width-2, height - 5);
    fill();
    closePath();

    // text
    beginPath();
    fillColor(foreground_color);
    fontSize(font_size);
    textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
    text(std::round(width / 2), std::round(height / 2.0f), buttonText.c_str(), NULL);
    closePath();

    // indicator
    if (has_mouse_ || isActive)
    {
        beginPath();
        lineCap(DGL::NanoVG::LineCap::ROUND);
        strokeColor(highlight_color);
        strokeWidth(4);
        moveTo(4, 4);
        lineTo(width - 4, 4);
        stroke();
        closePath();
    }
}

void TextButton::setText(std::string str)
{
    buttonText = str;
}
void TextButton::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
