#ifndef MYLINE_H
#define MYLINE_H

#include "ishape.h"

class MyLine : public IShape
{
public:
    MyLine();
    MyLine(int x1, int y1, int x2, int y2);
    int x1, x2, y1, y2;

    void swapAB() {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    std::vector<PixelWithColor> getPixels() override {
        std::vector<PixelWithColor> pixels;

        int dx = x2 - x1;
        int dy = y2 - y1;
        int d = 2*dy - dx; // initial value of d
        int dE = 2*dy; // increment used when moving to E
        int dNE = 2*(dy - dx); // increment used when movint to NE
        int x = x1, y = y1;

        pixels.push_back(PixelWithColor(x, y, R, G, B));

        while (x < x2)
        {
            if ( d < 0 ) { // move to E
                d += dE;
                x++;
            } else { // move to NE
                d += dNE;
                ++x;
                ++y;
            }
            pixels.push_back(PixelWithColor(x, y, R, G, B));
        }

        return pixels;
    }

    /*
    std::function<PixelWithColor()> generator = [this]{
        int dx = x2 - x1;
        int dy = y2 - y1;

        int d = dy - (dx/2);
        int x = x1, y = y2;

        return [=]() mutable {
            return PixelWithColor(x, y, R, G, B);

            while (x < x2) {
                x++;

                if (d < 0) {
                    d = d + dy;
                } else {
                    d += (dy - dx);
                    y++;
                }

                return PixelWithColor(x, y, R, G, B);
            }

            return PixelWithColor(-1, -1, -1, -1, -1);
        };
    }();*/


};

#endif // MYLINE_H
