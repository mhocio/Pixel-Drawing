#include "myline.h"

MyLine::MyLine()
{
    R = 0;
    G = 0;
    B = 0;
}

MyLine::MyLine(int x1, int y1, int x2, int y2) {
    R = 0;
    G = 0;
    B = 0;

    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
}
