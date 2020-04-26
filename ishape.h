#ifndef ISHAPE_H
#define ISHAPE_H

#include <iostream>
#include <vector>
#include <QString>
#include <string>
#include "pixelwithcolor.h"

class IShape
{
public:
    //std::function<PixelWithColor()> generator;
    virtual std::vector<PixelWithColor> getPixels() = 0;
    virtual std::string ToString() const;

    friend bool operator== (const IShape &c1, const IShape &c2);
    friend bool operator!= (const IShape &c1, const IShape &c2);

    int R;
    int G;
    int B;
    virtual ~IShape() {}
};

#endif // ISHAPE_H
