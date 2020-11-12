#include "SVGImage.hpp"

SVGImage::SVGImage(NanoWidget *parent, char *svgData, float scale)
    : NanoVG(parent)
{
    NSVGimage *svg = nsvgParse(svgData, "px", 96.0f);
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
