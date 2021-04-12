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
#include "FileOpenButton.hpp"

START_NAMESPACE_DISTRHO

FileOpenButton::FileOpenButton(Window &parent) noexcept
    : NanoWidget(parent)
{
    buttonText = "click to load sample";
    background_color = Color(0.8f, 0.8f, 0.8f);
    text_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    Roboto_ = createFontFromMemory("Roboto",
                                   reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                   fonts::Roboto_RegularDataSize, false);
}

FileOpenButton::FileOpenButton(Widget *widget) noexcept
    : NanoWidget(widget)
{
    buttonText = "click to load sample";
    background_color = Color(0.8f, 0.8f, 0.8f);
    text_color = Color(0.1f, 0.1f, 0.1f);
    font_size = 16;
    Roboto_ = createFontFromMemory("Roboto",
                                   reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                   fonts::Roboto_RegularDataSize, false);
    
}

bool FileOpenButton::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        callback->onFileOpenButtonClicked(this);
        return true;
    }
    else
    {
        return false;
    }
}

void FileOpenButton::onNanoDisplay()
{
    int width = getWidth();
    int height = getHeight();
    beginPath();
    fillColor(background_color);
    rect(0, 0, width, height);
    fill();
    closePath();

    // text
    beginPath();
    fillColor(text_color);
    fontSize(font_size);
    fontFaceId(Roboto_);
    textAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    Rectangle<float> bounds;
    textBounds(0, 0, buttonText.c_str(), NULL, bounds);
    std::string tempText = buttonText;
    for (int i = 0; i < buttonText.size(); i++) // maybe i = 1 ??
    {
        textBounds(0, 0, tempText.c_str(), NULL, bounds);
        // too large ?
        if (bounds.getWidth() > width)
        {
            // remove 1st character
            tempText = buttonText.substr(i);
        }
        else
            break;
    }

    text(0, std::round(height / 2.0f), tempText.c_str(), NULL);
    closePath();
}

void FileOpenButton::setText(std::string str)
{
    buttonText = str;
}
void FileOpenButton::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
