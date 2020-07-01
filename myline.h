#ifndef MYLINE_H
#define MYLINE_H

#include <cmath>
#include "ishape.h"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

class MyLine : public IShape
{
public:
    MyLine();
    MyLine(int x1, int y1, int x2, int y2);
    int x1, x2, y1, y2;

    std::string ToString() const override;
    json getJsonFormat() override;

    std::vector<PixelWithColor> getPixels() override {

        std::vector<PixelWithColor> pixels;

        int Ax = x1, Ay = y1, Bx = x2, By = y2;
        int	y,x,dy,dx,sx,sy;
        int	decision,incE,incNE;

        dx = Bx - Ax;
        dy = By - Ay;

        sx = sgn(dx);
        sy = sgn(dy);

        dx = std::abs(dx);
        dy = std::abs(dy);

        Bx += sx;
        By += sy;

        if(dy > dx) {  // vertical lines
            incE = 2 * dx;
            incNE = 2 * dx - 2 * dy;
            decision = 2 * dy - dx;

            x = Ax;
            y = Ay;
            do {
                pixels.push_back(PixelWithColor(x, y, R, G, B));
                for (int i = 1; i <= thickness - 1; i++) {
                    pixels.push_back(PixelWithColor(x + i, y, R, G, B));
                    pixels.push_back(PixelWithColor(x - i, y, R, G, B));
                }

                if(decision <= 0)
                    decision += incE;
                else{
                    decision += incNE;
                    x += sx;
                }
                y += sy;
            } while(y != By);
        } else {  // horizontal lines
            incE = 2 * dy;
            incNE = 2 * dy - 2 * dx;
            decision = 2 * dx - dy;

            x = Ax;
            y = Ay;
            do {
                pixels.push_back(PixelWithColor(x, y, R, G, B));
                for (int i = 1; i <= thickness - 1; i++) {
                    pixels.push_back(PixelWithColor(x, y + i, R, G, B));
                    pixels.push_back(PixelWithColor(x, y - i, R, G, B));
                }

                if (decision <= 0)
                    decision += incE;
                else {
                    decision += incNE;
                    y += sy;
                }
                x += sx;
            } while(x != Bx);
        }

    return pixels;
    }

    std::vector<PixelWithColor> getPixelsAliased() override {

        std::vector<PixelWithColor> pixels;

        // Xiaolin Wu's line algorithm
        // source: https://rosettacode.org/wiki/Xiaolin_Wu%27s_line_algorithm#C.2B.2B

        auto ipart = [](float x) -> int {return int(std::floor(x));};
            auto round = [](float x) -> float {return std::round(x);};
            auto fpart = [](float x) -> float {return x - std::floor(x);};
            auto rfpart = [=](float x) -> float {return 1 - fpart(x);};

            int xA = x1, xB = x2, yA = y1, yB = y2;

            const bool steep = abs(yB - yA) > abs(xB - xA);
            if (steep) {
                std::swap(xA,yA);
                std::swap(xB,yB);
            }
            if (xA > xB) {
                std::swap(xA,xB);
                std::swap(yA,yB);
            }

            const float dx = xB - xA;
            const float dy = yB - yA;
            const float gradient = (dx == 0) ? 1 : dy/dx;

            int xpx11;
            float intery;
            {
                const float xend = round(xA);
                const float yend = yA + gradient * (xend - xA);
                const float xgap = rfpart(xA + 0.5);
                xpx11 = int(xend);
                const int ypx11 = ipart(yend);
                if (steep) {
                    pixels.push_back(PixelWithColor(ypx11,     xpx11, R, G, B, rfpart(yend) * xgap));
                    pixels.push_back(PixelWithColor(ypx11 + 1, xpx11, R, G, B, fpart(yend) * xgap));
                } else {
                    pixels.push_back(PixelWithColor(xpx11, ypx11,  R, G, B,  rfpart(yend) * xgap));
                    pixels.push_back(PixelWithColor(xpx11, ypx11 + 1, R, G, B, fpart(yend) * xgap));
                }
                intery = yend + gradient;
            }

            int xpx12;
            {
                const float xend = round(xB);
                const float yend = yB + gradient * (xend - xB);
                const float xgap = rfpart(xB + 0.5);
                xpx12 = int(xend);
                const int ypx12 = ipart(yend);
                if (steep) {
                    pixels.push_back(PixelWithColor(ypx12,     xpx12, R, G, B, rfpart(yend) * xgap));
                    pixels.push_back(PixelWithColor(ypx12 + 1, xpx12, R, G, B, fpart(yend) * xgap));
                } else {
                    pixels.push_back(PixelWithColor(xpx12, ypx12,   R, G, B, rfpart(yend) * xgap));
                    pixels.push_back(PixelWithColor(xpx12, ypx12 + 1, R, G, B, fpart(yend) * xgap));
                }
            }

            if (steep) {
                for (int x = xpx11 + 1; x < xpx12; x++) {
                    pixels.push_back(PixelWithColor(ipart(intery),     x, R, G, B, rfpart(intery)));
                    pixels.push_back(PixelWithColor(ipart(intery) + 1, x, R, G, B, fpart(intery)));
                    intery += gradient;
                }
            } else {
                for (int x = xpx11 + 1; x < xpx12; x++) {
                    pixels.push_back(PixelWithColor(x, ipart(intery),    R, G, B, rfpart(intery)));
                    pixels.push_back(PixelWithColor(x, ipart(intery) + 1, R, G, B, fpart(intery)));
                    intery += gradient;
                }
            }



        return pixels;
    }
};

#endif // MYLINE_H
