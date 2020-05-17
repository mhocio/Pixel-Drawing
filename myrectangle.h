#ifndef MYRECTANGLE_H
#define MYRECTANGLE_H

#include "ishape.h"
#include "myline.h"
#include "mypolygon.h"

class myRectangle : public myPolygon
{
public:
    myRectangle();
    myRectangle(std::pair<int, int> A, std::pair<int, int> B);

    std::pair<int, int> firstPoint, secondPoint;
};

#endif // MYRECTANGLE_H
