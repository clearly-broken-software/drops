#include "SVGImage.hpp"
 

SVGImage::SVGImage(char *svgData, float scale)
{
    NSVGimage *svg = nsvgParse(svgData, "px", 96.0f);
    if (svg == nullptr)
    {
        printf("Could not open SVG Image.\n");
        return;
    }
    width = static_cast<uint>(svg->width);
    height = static_cast<uint>(svg->height);
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
    delete imgData;
}
