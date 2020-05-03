#ifndef PIXELWITHCOLOR_H
#define PIXELWITHCOLOR_H


class PixelWithColor
{
public:
    PixelWithColor(int x, int y, int R, int G, int B);
    PixelWithColor(int x, int y, int R, int G, int B, double I);
    int x;
    int y;

    int R;
    int G;
    int B;

    double intensity;
};

#endif // PIXELWITHCOLOR_H
