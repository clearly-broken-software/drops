/*  license  */

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
