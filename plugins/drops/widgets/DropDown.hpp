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
#ifndef DROP_DOWN_HPP
#define DROP_DOWN_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include "Menu.hpp"
#include "fonts.hpp"
#include <string>

START_NAMESPACE_DISTRHO

class DropDown : public NanoWidget,
                 public IdleCallback
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void onDropDownClicked(DropDown *dropDown) = 0;
  };
  explicit DropDown(Window &parent) noexcept;
  explicit DropDown(Widget *widget) noexcept;
  void setValue(float value);

  void addOption(std::string str);
  float getMenuOffset();
  void setCallback(Callback *cb);
  void setMenu(Menu *menu);
  void setFont(const char *name, const uchar *data, uint dataSize);
  void setMenuFont(const char *name, const uchar *data, uint dataSize);
  void positionMenu();
  void resize();
  void idleCallback() override;
  std::string label;
  std::string item;
  float font_size;
  float margin;
  Color background_color, foreground_color, highlight_color, text_color;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;

private:
  Callback *callback_;
  bool has_mouse_;
  Menu *menu_;
  FontId main_font_;
  FontId menu_font_;

  DISTRHO_LEAK_DETECTOR(DropDown)
};

END_NAMESPACE_DISTRHO

#endif // TEXT_BUTTON_HPP
