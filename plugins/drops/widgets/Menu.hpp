/*  license  */

#ifndef MENU_HPP
#define MENU_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include <string>
#include <sstream>

START_NAMESPACE_DISTRHO

class Menu : public NanoWidget,
             public IdleCallback
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void menuClicked(Menu *menu, uint index, std::string item) = 0;
  };
  explicit Menu(Window &parent) noexcept;
  void addItem(std::string str);
  void setCallback(Callback *cb);
  void idleCallback();

  float font_size;
  float margin;
  int timer, time_out;
  bool dropdown_has_mouse;
  Color background_color;
  Color foreground_color;
  Color highlite_color;
  Color text_color;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;

private:
  std::vector<std::string> items_;
  std::string getItem(uint index);
  void resize();
  Callback *callback_;
  int highlighted_item_;
  bool has_mouse_;
  
  

  DISTRHO_LEAK_DETECTOR(Menu)
};

END_NAMESPACE_DISTRHO

#endif
