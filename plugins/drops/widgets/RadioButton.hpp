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
#ifndef RADIO_BUTTON_HPP
#define RADIO_BUTTON_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class RadioButton : public NanoWidget
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void onRadioButtonClicked(RadioButton *rb) = 0;
  };
  explicit RadioButton(Window &parent) noexcept;
  explicit RadioButton(Widget *widget) noexcept;
  void addOption(const char *opt);
  void setCallback(Callback *cb);
  uint active_option;
  float font_size;
  float margin;
  Color background_color, foreground_color, highlight_color, text_color;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;

private:
  Callback *callback_;

  std::vector<const char *> options;

  DISTRHO_LEAK_DETECTOR(RadioButton)
};

END_NAMESPACE_DISTRHO

#endif
