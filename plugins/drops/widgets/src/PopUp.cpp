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
#include "PopUp.hpp"

START_NAMESPACE_DISTRHO

PopUp::PopUp(Window &parent) noexcept
    : NanoWidget(parent)
{
    popupText = "Popup text";
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    margin = 4.0f;
}

PopUp::PopUp(Widget *widget) noexcept
    : NanoWidget(widget)
{
    popupText = "Popup text";
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    margin = 4.0f;
}

void PopUp::onNanoDisplay()
{
    // check if still in display
    int width = getWidth();
    int height = getHeight();
    if ((getAbsoluteY() + height) > UI_H)
    {
        setAbsoluteY(UI_H - height);
    }

    beginPath();
    strokeColor(foreground_color);
    fillColor(background_color);
    strokeWidth(2.0f);
    roundedRect(1, 1, width - 2, height - 2, 2);
    fill();
    stroke();
    closePath();

    // text
    beginPath();
    fillColor(text_color);
    fontFaceId(mainFont);
    fontSize(font_size);
    textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
    text(std::round(width / 2), std::round(height / 2.0f), popupText.c_str(), NULL);
    closePath();
}

void PopUp::setText(std::string str)
{
    popupText = str;
}

void PopUp::setFont(const char *name, const uchar *data, uint dataSize)
{
    mainFont = createFontFromMemory(name, data, dataSize, false);
}

void PopUp::resize()
{
    // measure string
    fontFaceId(mainFont);
    fontSize(font_size);
    Rectangle<float> bounds;
    textBounds(0, 0, popupText.c_str(), nullptr, bounds);
    setSize(bounds.getWidth() + 0.5f + margin, bounds.getHeight() + 0.5f + margin);
}

END_NAMESPACE_DISTRHO
