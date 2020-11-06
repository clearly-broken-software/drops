/* license */
#ifndef SVG_IMAGE_HPP_INCLUDED
#define SVG_IMAGE_HPP_INCLUDED
#include "NanoVG.hpp"
#include "nanosvg.h"
#include "nanosvgrast.h"

START_NAMESPACE_DISTRHO

class SVGImage : public NanoVG
{
public:
    SVGImage(char *svg, float scale);

    ~SVGImage();

    NanoImage image;
    Size<uint> getSize()
    {
        return Size<uint>(width, height);
    };
    uint getWidth()
    {
        return width;
    };
    uint getHeight()
    {
        return height;
    };
    void setScale(float s)
    {
        scale = s;
    };

private:
    unsigned char *imgData;
    Size<uint> size;
    uint width;
    uint height;
    float scale;
};

END_NAMESPACE_DISTRHO

#endif