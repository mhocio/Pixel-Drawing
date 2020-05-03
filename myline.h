#ifndef MYLINE_H
#define MYLINE_H

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

    void swapAB() {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }


    void swap(int* a , int*b)
    {
        int temp = *a;
        *a = *b;
        *b = temp;
    }

    // returns absolute value of number
    float absolute(float x )
    {
        if (x < 0) return -x;
        else return x;
    }

    //returns integer part of a floating point number
    int iPartOfNumber(float x)
    {
        return (int)x;
    }

    //rounds off a number
    int roundNumber(float x)
    {
        return iPartOfNumber(x + 0.5) ;
    }

    //returns fractional part of a number
    float fPartOfNumber(float x)
    {
        if (x>0) return x - iPartOfNumber(x);
        else return x - (iPartOfNumber(x)+1);

    }

    //returns 1 - fractional part of number
    float rfPartOfNumber(float x)
    {
        return 1 - fPartOfNumber(x);
    }

    std::string ToString() const override;

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

        bool steep = absolute(y2 - y1) > absolute(x2 - x1);
        int xa = x1, xb = x2, ya = y1, yb = y2;

        // swap the co-ordinates if slope > 1 or we
        // draw backwards
        if (steep)
        {
            swap(&xa , &ya);
            swap(&xb , &yb);
        }
        if (x1 > x2)
        {
            swap(&xa ,&xb);
            swap(&ya ,&yb);
        }

        //compute the slope
        float dx = xb-xa;
        float dy = yb-ya;
        float gradient = dy/dx;
        if (dx == 0.0)
            gradient = 1;

        int xpxl1 = xa;
        int xpxl2 = xb;
        float intersectY = y1;

        // main loop
        if (steep)
        {
            int x;
            for (x = xpxl1 ; x <=xpxl2 ; x++)
            {
                // pixel coverage is determined by fractional
                // part of y co-ordinate
                //pixels.push_back(PixelWithColor(x, y, R, G, B));
                pixels.push_back(PixelWithColor(iPartOfNumber(intersectY), x, R, G, B,
                            rfPartOfNumber(intersectY)));
                pixels.push_back(PixelWithColor(iPartOfNumber(intersectY)-1, x, R, G, B,
                            fPartOfNumber(intersectY)));
                intersectY += gradient;
            }
        }
        else
        {
            int x;
            for (x = xpxl1 ; x <=xpxl2 ; x++)
            {
                // pixel coverage is determined by fractional
                // part of y co-ordinate
                pixels.push_back(PixelWithColor(x, iPartOfNumber(intersectY), R, G, B,
                            rfPartOfNumber(intersectY)));
                pixels.push_back(PixelWithColor(x, iPartOfNumber(intersectY)-1, R, G, B,
                              fPartOfNumber(intersectY)));
                intersectY += gradient;
            }
        }
        return pixels;
    }
};

#endif // MYLINE_H
