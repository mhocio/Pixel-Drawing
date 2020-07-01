#ifndef ISHAPE_H
#define ISHAPE_H

#include <iostream>
#include <vector>
#include <QString>
#include <QColor>
#include <string>
#include "pixelwithcolor.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class IShape
{
public:
    //std::function<PixelWithColor()> generator;
    virtual std::vector<PixelWithColor> getPixels() = 0;
    // how to pass to a lambda function
    virtual std::vector<PixelWithColor> getPixelsAliased() = 0;

    virtual json getJsonFormat() = 0;
    json getIShapeJson();

    virtual std::string ToString() const;

    friend bool operator== (const IShape &c1, const IShape &c2);
    friend bool operator!= (const IShape &c1, const IShape &c2);

    virtual void setColor(QColor color);

    int R;
    int G;
    int B;

    int thickness = 1;

    void setThickness(int t) {
        thickness = t;
    }

    virtual ~IShape() {}
};

#endif // ISHAPE_H
