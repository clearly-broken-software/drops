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
#ifndef POPUP_HPP
#define POPUP_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include "DropsGeometry.hpp"
#include <string>

START_NAMESPACE_DISTRHO

class PopUp : public NanoWidget
{
public:
  explicit PopUp(Window &parent) noexcept;
  explicit PopUp(Widget *widget) noexcept;
  void setText(std::string str);
  void setFont(const char *name, const uchar *data, uint dataSize);
  void resize();
  Color background_color;
  Color foreground_color;
  Color text_color;
  float font_size;
  float margin;
  std::string popupText;

protected:
  void onNanoDisplay() override;
 

private:
  
  Color fill_color_;
  FontId mainFont;

  DISTRHO_LEAK_DETECTOR(PopUp)
};

END_NAMESPACE_DISTRHO

#endif // TEXT_BUTTON_HPP
