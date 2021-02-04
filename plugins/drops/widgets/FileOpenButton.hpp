/*  license  */

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
