/*  license  */

#ifndef POPUP_HPP
#define POPUP_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include "DropsGeometry.hpp"
#include <string>

START_NAMESPACE_DISTRHO

class PopUp : public NanoWidget
{
public:
  explicit PopUp(Window &parent) noexcept;
  explicit PopUp(Widget *widget) noexcept;
  void setText(std::string str);
  void setFont(const char *name, const uchar *data, uint dataSize);
  void resize();
  Color background_color;
  Color foreground_color;
  Color text_color;
  float font_size;
  float margin;
  std::string popupText;

protected:
  void onNanoDisplay() override;
 

private:
  
  Color fill_color_;
  FontId mainFont;

  DISTRHO_LEAK_DETECTOR(PopUp)
};

END_NAMESPACE_DISTRHO

#endif // TEXT_BUTTON_HPP
