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

    std::string ToString() const override;

    std::vector<PixelWithColor> getPixels() override {

        std::vector<PixelWithColor> pixels;
/*
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
    }*/

    //void drawLine(int Ax,int Ay,int Bx,int By)
    //{
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

        if(dy > dx)
        {
            incE = 2 * dx;
            incNE = 2 * dx - 2 * dy;
            decision = 2 * dy - dx;

            x = Ax;
            y = Ay;
            do{
                //setCell(x,y,0.4,0.7,1.0);
                pixels.push_back(PixelWithColor(x, y, R, G, B));
                if(decision <= 0)
                    decision += incE;
                else{
                    decision += incNE;
                    x += sx;
                }
                y += sy;
            }while(y != By);
        }else{
            incE = 2 * dy;
            incNE = 2 * dy - 2 * dx;
            decision = 2 * dx - dy;

            x = Ax;
            y = Ay;
            do{
                //setCell(x,y,0.4,0.7,1.0);
                pixels.push_back(PixelWithColor(x, y, R, G, B));
                if(decision <= 0)
                    decision += incE;
                else{
                    decision += incNE;
                    y += sy;
                }
                x += sx;
            }while(x != Bx);
        }
    //}

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

    return pixels;
    }
};

#endif // MYLINE_H
