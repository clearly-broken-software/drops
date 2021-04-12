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
#include "RadioButton.hpp"

START_NAMESPACE_DISTRHO

RadioButton::RadioButton(Window &parent) noexcept
    : NanoWidget(parent)
{
    loadSharedResources();
    font_size = 14.f;
    margin = 4.f;
    background_color = Color(0.0f, 0.0f, 0.0f);
    foreground_color = Color(0.5f, 0.5f, 0.5f);
    highlight_color = Color(1.0f, 0.0f, 0.0f);
    text_color = Color(1.0f, 1.0f, 1.0f);
    active_option = 0;
}
RadioButton::RadioButton(Widget *widget) noexcept
    : NanoWidget(widget)
{
    loadSharedResources();
    font_size = 14.f;
    margin = 4.f;
    background_color = Color(0.0f, 0.0f, 0.0f);
    foreground_color = Color(0.5f, 0.5f, 0.5f);
    highlight_color = Color(1.0f, 0.0f, 0.0f);
    text_color = Color(1.0f, 1.0f, 1.0f);
    active_option = 0;
}

bool RadioButton::onMouse(const MouseEvent &ev)
{
    if (!isVisible())
        return false;

    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        const int lineHeight = font_size + 2 * margin;
        const int mouseY = ev.pos.getY();
        int step = 0;
        const uint old_option = active_option;
        for (int i = 0; i < options.size(); i++)
        {
            if ((mouseY >= i * lineHeight) && (mouseY < (i * lineHeight + lineHeight)))
                active_option = i;
        }
        if (old_option != active_option)
        {
            callback_->onRadioButtonClicked(this);
            repaint();
        }
        return true;
    }
    else
    {
        return false;
    }
}

void RadioButton::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();
    const uint x = getAbsoluteX();
    const uint y = getAbsoluteY();
    // background
    beginPath();
    fillColor(background_color);
    rect(0, 0, width, height);
    fill();
    closePath();

    strokeColor(foreground_color);
    Color fill_color = background_color;
    strokeWidth(margin);
    const float lineHeight = font_size + 2 * margin;
    float step_y = 0.0f;
    const float cx = margin + (font_size / 2.0f);
    const float cy = lineHeight / 2.0f;
    for (int i = 0; i < options.size(); i++)
    {
        beginPath();
        circle(cx, cy + step_y, font_size / 2.0f);
        fill_color = i == active_option ? highlight_color : background_color;
        fillColor(fill_color);
        fill();
        closePath();
        beginPath();
        circle(cx, cy + step_y, (font_size / 2.0f) - margin / 2.0f);
        stroke();
        closePath();

        fillColor(text_color);
        fontSize(font_size);
        textAlign(ALIGN_LEFT | ALIGN_MIDDLE);
        beginPath();
        text(lineHeight + margin, step_y + lineHeight / 2.0f + margin, options[i], nullptr);
        closePath();
        step_y += lineHeight;
    }
}

void RadioButton::addOption(const char *op)
{
    options.push_back(op);
}

void RadioButton::setCallback(Callback *cb)
{
    callback_ = cb;
}

END_NAMESPACE_DISTRHO
