/* license */
#include "SVGButton.hpp"

START_NAMESPACE_DISTRHO

SVGButton::SVGButton(Window &parent, char *svg, char *svgHL) noexcept
    : NanoWidget(parent)
{
    svgImage = new SVGImage(this, svg, 1.0f);
    if (svgHL)
        svgImageHL = new SVGImage(this, svgHL, 1.0f);
    setSize(svgImage->getSize());

    has_mouse_ = false;
}

SVGButton::SVGButton(Widget *widget, char *svg, char *svgHL) noexcept
    : NanoWidget(widget)
{
    svgImage = new SVGImage(this, svg, 1.0f);
    if (svgHL)
        svgImageHL = new SVGImage(this, svgHL, 1.0f);
    setSize(svgImage->getSize());

    has_mouse_ = false;
}

bool SVGButton::onMouse(const MouseEvent &ev)
{
    if (contains(ev.pos) && ev.press && ev.button == 1)
    {
        callback->onSVGButtonClicked(this);
        return true;
    }
    else
    {
        return false;
    }
}
bool SVGButton::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos) && !has_mouse_)
    {
        has_mouse_ = true;
        repaint();
    }
    if (!contains(ev.pos) && has_mouse_)
    {
        has_mouse_ = false;
        repaint();
    }
    return false;
}

void SVGButton::onNanoDisplay()
{
    if (has_mouse_)
        svgImageHL->drawAt(0, 0);
    else
        svgImage->drawAt(0, 0);
    // int width = getWidth();
    // int height = getHeight();
    // beginPath();
    // fill_color_ = has_mouse_ ? highlight_color : background_color;
    // fillColor(fill_color_);
    // rect(0, 0, width, height);
    // fill();
    // closePath();
}

void SVGButton::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO
