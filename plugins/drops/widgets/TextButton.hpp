/*  license  */

#ifndef TEXT_BUTTON_HPP
#define TEXT_BUTTON_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"

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
  void setCallback(Callback *cb);

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent&) override;

private:
  Callback *callback;

  DISTRHO_LEAK_DETECTOR(TextButton)
};

END_NAMESPACE_DISTRHO

#endif // TEXT_BUTTON_HPP
