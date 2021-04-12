/*
    Drops - Drops Really Only Plays Samples
    Copyright (C) 2021  Rob van den Berg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef SVG_IMAGE_HPP_INCLUDED
#define SVG_IMAGE_HPP_INCLUDED
#include "NanoVG.hpp"
#include "nanosvg.h"
#include "nanosvgrast.h"

START_NAMESPACE_DISTRHO

class SVGImage : public NanoVG
{
public:
    SVGImage(NanoWidget *parent, char *svg, float scale);

    ~SVGImage();

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

    void drawAt(int x, int y);
    
    NanoImage image;

private:
    unsigned char *imgData;
    Size<uint> size;
    uint width;
    uint height;
    Paint paint;
    float scale;

};

END_NAMESPACE_DISTRHO

#endif