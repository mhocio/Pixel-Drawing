#ifndef ISHAPE_H
#define ISHAPE_H

#include <iostream>
#include <vector>
#include "pixelwithcolor.h"

class IShape
{
public:
    //std::function<PixelWithColor()> generator;
    virtual std::vector<PixelWithColor> getPixels() = 0;

    int R;
    int G;
    int B;
    virtual ~IShape() {}
};

#endif // ISHAPE_H
