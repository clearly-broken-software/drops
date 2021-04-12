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
#include "SVGImage.hpp"

SVGImage::SVGImage(NanoWidget *parent, char *svgData, float scale)
    : NanoVG(parent)
{
    char* tmpSVG = strdup(svgData);
    NSVGimage *svg = nsvgParse(tmpSVG, "px", 96.0f);
    if (svg == nullptr)
    {
        printf("Could not open SVG Image.\n");
        return;
    }
    width = static_cast<uint>(svg->width * scale + 0.5f);
    height = static_cast<uint>(svg->height * scale + 0.5f);
    size_t s = width * height * 4;
    imgData = new unsigned char[s];
    NSVGrasterizer *rast;
    rast = nsvgCreateRasterizer();
    if (rast == nullptr)
    {
        printf("Could not init rasterizer.\n");
        return;
    }

    nsvgRasterize(rast, svg, 0, 0, scale, imgData, width, height, width * 4);
    image = createImageFromRGBA(width, height, imgData, IMAGE_GENERATE_MIPMAPS);
    // cleanup
    nsvgDelete(svg);
    nsvgDeleteRasterizer(rast);
    free(tmpSVG);
};

SVGImage::~SVGImage()
{
    delete[] imgData;
}

void SVGImage::drawAt(int x, int y)
{
    paint = imagePattern(x, y, width, height, 0, image, 1.0f);
    beginPath();
    rect(x, y, width, height);
    fillPaint(paint);
    fill();
    closePath();
}
