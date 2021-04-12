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
#ifndef FILE_OPEN_BUTTON_HPP
#define FILE_OPEN_BUTTON_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include "fonts.hpp"
#include <string>

START_NAMESPACE_DISTRHO

class FileOpenButton : public NanoWidget
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void onFileOpenButtonClicked(FileOpenButton *fileOpenButton) = 0;
  };
  explicit FileOpenButton(Window &parent) noexcept;
  explicit FileOpenButton(Widget *widget) noexcept;
  void setText(std::string str);
  void setCallback(Callback *cb);
  Color background_color;
  Color text_color;
  float font_size;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;

private:
  std::string buttonText;
  Callback *callback;
  FontId Roboto_;

  DISTRHO_LEAK_DETECTOR(FileOpenButton)
};

END_NAMESPACE_DISTRHO

#endif // FILE_OPEN_BUTTON_HPP
