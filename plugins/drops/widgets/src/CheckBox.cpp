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
#include "CheckBox.hpp"

START_NAMESPACE_DISTRHO

CheckBox::CheckBox(Window &parent) noexcept
    : NanoWidget(parent)
{
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    highlight_color_active = Color(1.0f, 0.5f, 0.5f);
    highlight_color_inactive = Color(0.0f, 1.0f, 0.0f);
    has_mouse_ = false;
    isActive = false;
}

CheckBox::CheckBox(Widget *widget) noexcept
    : NanoWidget(widget)
{
    background_color = Color(0.8f, 0.8f, 0.8f);
    foreground_color = Color(0.1f, 0.1f, 0.1f);
    highlight_color_active = Color(1.0f, 0.5f, 0.5f);
    highlight_color_inactive = Color(0.0f, 1.0f, 0.0f);

    has_mouse_ = false;
    isActive = false;
}

bool CheckBox::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        isActive = !isActive;
        callback->onCheckBoxClicked(this, isActive);
        repaint();
        return true; // TODO check if this should be true/false
    }
    else
    {
        return false;
    }
}
bool CheckBox::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos) && !has_mouse_)
    {
        has_mouse_ = true; // TODO check if this should be true/false
    }
    repaint();

    if (!contains(ev.pos) && has_mouse_)
    {
        has_mouse_ = false;
        repaint();
    }
    return false;
}

void CheckBox::onNanoDisplay()
{
    float width = getWidth();
    float height = getHeight();
    float label_x,
          label_y,
          box_x,
          box_y;

    // label
    fontFaceId(main_font_);
    fontSize(labelSize);
    Rectangle<float> bounds;
    textBounds(0.f, 0.f, label, NULL, bounds);
    const float label_height = bounds.getHeight();
    // unused const float label_width  = bounds.getWidth();

    if(labelOnRightSide)
    {
        label_x = boxSize + margin + margin ; // checkbox width + 5px;
        label_y = 0.f ;
        textAlign(ALIGN_LEFT | ALIGN_TOP);
    }
    else
    {
        label_x = width * .5f; //- label_width / 2.0f;
        label_y = height - label_height;
        textAlign(ALIGN_CENTER | ALIGN_TOP);
    }
    beginPath();
    fillColor(text_color);
    text(label_x, label_y, label, NULL);
    closePath();

    //box
    if (has_mouse_ && isActive)
    {
        fill_color_ = highlight_color_active;
    }
    else if (has_mouse_&&!isActive)
    {
        fill_color_ = highlight_color_inactive;
    }
    
    else if (isActive)
    {
        fill_color_ = foreground_color;
    }
    else
    {
        fill_color_ = background_color;
    }

    beginPath();
    fillColor(fill_color_);

    if (labelOnRightSide) {
        box_x = margin;
        box_y = height - boxSize - margin - margin;
#ifdef DEBUG
        printf("\"x: %f,y: %f, width: %f, height: %f \" // \n", box_x, box_y, boxSize + 5 + width, boxSize - 2.f);
#endif
        roundedRect(box_x + 1.f, box_y ,
                    boxSize - 2.f, boxSize - 2.f,
                    2.f);
    }
    else
    {
        box_x = width / 2.f - boxSize / 2.f;
        box_y = height - label_height - boxSize - margin;

        roundedRect(box_x + 1.f, box_y + 1.f,
                    boxSize - 2.f, boxSize - 2.f,
                    2.f);
    }

    fill();
    closePath();
}

void CheckBox::setCallback(Callback *cb)
{
    callback = cb;
}
void CheckBox::setFont(const char *fontName, const uchar *data, uint dataSize)
{
    main_font_ = createFontFromMemory(fontName, data, dataSize, false);
}

END_NAMESPACE_DISTRHO
