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
  explicit SVGButton(Window &parent, char *svg, char *svgHL) noexcept;
  explicit SVGButton(Widget *widget, char *svg, char *svgHL) noexcept;
  void setCallback(Callback *cb);

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;

private:
  bool has_mouse_;
  Callback *callback;
  ScopedPointer<SVGImage> svgImage,svgImageHL;

  DISTRHO_LEAK_DETECTOR(SVGButton)
};

END_NAMESPACE_DISTRHO

#endif // SVG_BUTTON_HPP
