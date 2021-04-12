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
#ifndef MENU_HPP
#define MENU_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include "fonts.hpp"
#include "DropsGeometry.hpp"
#include <string>
#include <sstream>
#include <initializer_list>

START_NAMESPACE_DISTRHO

class Menu : public NanoWidget,
             public IdleCallback
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void onMenuClicked(Menu *menu, uint index, std::string item) = 0;
  };
  explicit Menu(Window &parent) noexcept;
  explicit Menu(Widget *widget) noexcept;
  void addItems(std::initializer_list<const char *> items);
  void setCallback(Callback *cb);
  void idleCallback();
  std::string getItem(uint index);
  void setFont(const char *name, const uchar *data, uint dataSize);
  void setMaxViewItems(int maxViewItems);

  float font_size;
  float margin;
  int timer, time_out;
  bool dropdown_has_mouse;
  int item_view_first, item_view_last;

  Color background_color;
  Color foreground_color;
  Color highlight_color;
  Color text_color;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;
  bool onScroll(const ScrollEvent &) override;

private:
  void resize();
  void scrollMenu(int dir);

  std::vector<const char *> items_;

  Callback *callback_;
  int highlighted_item_;
  bool has_mouse_;
  bool max_size_reached_;

  int max_view_items; // number of items in a menu

  FontId menu_font_;

  DISTRHO_LEAK_DETECTOR(Menu)
};

END_NAMESPACE_DISTRHO

#endif
