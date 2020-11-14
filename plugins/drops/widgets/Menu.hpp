/*  license  */

#ifndef MENU_HPP
#define MENU_HPP

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include <string>
#include <sstream>
#include <initializer_list>

START_NAMESPACE_DISTRHO

class Menu : public NanoWidget,
             public IdleCallback
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void onMenuClicked(Menu *menu, uint index, std::string item) = 0;
  };
  explicit Menu(Window &parent) noexcept;
  explicit Menu(Widget *widget) noexcept;
  void addItems(std::initializer_list<const char *> items);
  void setCallback(Callback *cb);
  void idleCallback();

  float font_size;
  float margin;
  int timer, time_out;
  bool dropdown_has_mouse;
  int max_view_items; // number of items in a menu
  int item_view_first, item_view_last;

  Color background_color;
  Color foreground_color;
  Color highlight_color;
  Color text_color;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;
  bool onScroll(const ScrollEvent &) override;

private:
  void resize();
  void scrollMenu(int dir);

  std::vector<const char *> items_;
  std::string getItem(uint index);

  Callback *callback_;
  int highlighted_item_;
  bool has_mouse_;
  bool max_size_reached_;

  DISTRHO_LEAK_DETECTOR(Menu)
};

END_NAMESPACE_DISTRHO

#endif
