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
#ifndef CHECK_BOX_HPP
#define CHECK_BOX_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class CheckBox : public NanoWidget
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void onCheckBoxClicked(CheckBox *checkBox, bool is_checked) = 0;
  };
  explicit CheckBox(Window &parent) noexcept;
  explicit CheckBox(Widget *widget) noexcept;
  void setCallback(Callback *cb);
  void setFont(const char *name, const uchar *data, uint dataSize);
  Color background_color;
  Color foreground_color;
  Color highlight_color_active;
  Color highlight_color_inactive;
  Color text_color;
  float labelSize;
  float margin;
  float boxSize;
  const char *label;
  bool isActive;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;

private:
  bool is_checked_;
  bool has_mouse_;
  Callback *callback;
  Color fill_color_;
  FontId main_font_;

  DISTRHO_LEAK_DETECTOR(CheckBox)
};

END_NAMESPACE_DISTRHO

#endif // TEXT_BUTTON_HPP
