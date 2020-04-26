#include "myline.h"

MyLine::MyLine()
{
    this->R = 0;
    this->G = 0;
    this->B = 0;
}

MyLine::MyLine(int x1, int y1, int x2, int y2) {
    this->R = 0;
    this->G = 0;
    this->B = 0;

    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
}
