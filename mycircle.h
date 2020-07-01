#ifndef MYCIRCLE_H
#define MYCIRCLE_H

#include <QtDebug>
#include "ishape.h"

class MyCircle : public IShape
{
public:
    MyCircle();
    MyCircle(int X, int Y, int radius);
    int X, Y, radius;

    std::string ToString() const override;
    json getJsonFormat() override;

    std::vector<PixelWithColor> getPixels() override {
        std::vector<PixelWithColor> pixels;

        int x = radius;
        int y = 0;
        int err = 0;

        while (x >= y)
        {
            pixels.push_back(PixelWithColor(X + x, Y + y, R, G, B));
            pixels.push_back(PixelWithColor(X + y, Y + x, R, G, B));
            pixels.push_back(PixelWithColor(X - y, Y + x, R, G, B));
            pixels.push_back(PixelWithColor(X - x, Y + y, R, G, B));
            pixels.push_back(PixelWithColor(X - x, Y - y, R, G, B));
            pixels.push_back(PixelWithColor(X - y, Y - x, R, G, B));
            pixels.push_back(PixelWithColor(X + y, Y - x, R, G, B));
            pixels.push_back(PixelWithColor(X + x, Y - y, R, G, B));

            if (err <= 0) {
                y++;
                err += 2*y + 1;
            }

            if (err > 0) {
                x--;
                err -= 2*x + 1;
            }
        }
/*
        //void midPointCircleDraw(int x_centre, int y_centre, int r)
        int x = radius, y = 0;
        int x_centre = X;
        int y_centre = Y;

        pixels.push_back(PixelWithColor(x + x_centre, y + y_centre, R, G, B));

        if (radius > 0)
        {
            pixels.push_back(PixelWithColor(x + x_centre, -y + y_centre, R, G, B));
            pixels.push_back(PixelWithColor(y + x_centre, x + y_centre , R, G, B));
            pixels.push_back(PixelWithColor(-y + x_centre, x + y_centre, R, G, B));
        }

        int P = 1 - radius;
        while (x > y)
        {
            y++;

            if (P <= 0)
                P = P + 2*y + 1;
            else {
                x--;
                P = P + 2*y - 2*x + 1;
            }

            if (x < y)
                break;

            pixels.push_back(PixelWithColor(x + x_centre, y + y_centre  , R, G, B));
            pixels.push_back(PixelWithColor(-x + x_centre, y + y_centre , R, G, B));
            pixels.push_back(PixelWithColor(x + x_centre, -y + y_centre , R, G, B));
            pixels.push_back(PixelWithColor(-x + x_centre, -y + y_centre, R, G, B));

            if (x != y)
            {
                pixels.push_back(PixelWithColor(y + x_centre, x + y_centre  , R, G, B));
                pixels.push_back(PixelWithColor(-y + x_centre, x + y_centre , R, G, B));
                pixels.push_back(PixelWithColor(y + x_centre, -x + y_centre , R, G, B));
                pixels.push_back(PixelWithColor(-y + x_centre, -x + y_centre, R, G, B));
            }
        }*/

        return pixels;
    }

    std::vector<PixelWithColor> getPixelsAliased() override {

        std::vector<PixelWithColor> pixels;
        return pixels;
    }
};

#endif // MYCIRCLE_H
