/*  license  */

#ifndef RADIO_BUTTON_HPP
#define RADIO_BUTTON_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
//#include <string>

START_NAMESPACE_DISTRHO

class RadioButton : public NanoWidget
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void onRadioButtonClicked(RadioButton * rb) = 0;
  };
  explicit RadioButton(Window &parent) noexcept;
  explicit RadioButton(Widget *widget) noexcept;
  void addOption(char *opt);
  void setCallback(Callback *cb);

  float font_size;
  float margin;
  Color background_color, foreground_color, highlight_color, text_color;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;

private:
  Callback *callback_;
  uint active_option;
  std::vector<char *> options;

  DISTRHO_LEAK_DETECTOR(RadioButton)
};

END_NAMESPACE_DISTRHO

#endif 
