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
    virtual void textButtonClicked(TextButton *textButton) = 0;
  };
  explicit TextButton(Window &parent, Size<uint> size) noexcept;
  explicit TextButton(Widget *widget, Size<uint> size) noexcept;
  void setText(std::string str);
  void setCallback(Callback *cb);
  Color back_ground_color;
  Color text_color;
  float font_size;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;

private:
  std::string buttonText;
  Callback *callback;

  DISTRHO_LEAK_DETECTOR(TextButton)
};

END_NAMESPACE_DISTRHO

#endif // TEXT_BUTTON_HPP
