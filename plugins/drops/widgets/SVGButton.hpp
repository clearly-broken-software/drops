/*  license  */

#ifndef SVG_BUTTON_HPP_INCLUDED
#define SVG_BUTTON_HPP_INCLUDED

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include "SVGImage.hpp"

START_NAMESPACE_DISTRHO

class SVGButton : public NanoWidget
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void onSVGButtonClicked(SVGButton *svgButton) = 0;
  };
  explicit SVGButton(Window &parent, char *svg, char *svgHL = nullptr, float scale = 1.0f) noexcept;
  explicit SVGButton(Widget *widget, char *svg, char *svgHL = nullptr, float scale = 1.0f) noexcept;
  void setCallback(Callback *cb);
  Color background_color;
  Color highlight_color;
  Color active_color;
  bool is_active;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;

private:
  
  bool has_mouse_;
  bool has_highlight;
  Callback *callback;
  ScopedPointer<SVGImage> svgImage,svgImageHL;
  Color fill_color_;

  DISTRHO_LEAK_DETECTOR(SVGButton)
};

END_NAMESPACE_DISTRHO

#endif // SVG_BUTTON_HPP
