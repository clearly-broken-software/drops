/*  license  */

#ifndef TEXT_BUTTON_HPP
#define TEXT_BUTTON_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include <string>

START_NAMESPACE_DISTRHO

class TextButton : public NanoWidget
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void onTextButtonClicked(TextButton *textButton) = 0;
  };
  explicit TextButton(Window &parent) noexcept;
  explicit TextButton(Widget *widget) noexcept;
  void setText(std::string str);
  void setCallback(Callback *cb);
  Color background_color;
  Color foreground_color;
  Color highlight_color;
  bool isActive;

  float font_size;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;

private:
  std::string buttonText;
  bool has_mouse_;
  Callback *callback;
  Color fill_color_;

  DISTRHO_LEAK_DETECTOR(TextButton)
};

END_NAMESPACE_DISTRHO

#endif // TEXT_BUTTON_HPP
