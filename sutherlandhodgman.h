#ifndef SUTHERLANDHODGMAN_H
#define SUTHERLANDHODGMAN_H

#include <QPoint>
#include <vector>
#include <algorithm>
#include "myline.h"
#include "mypolygon.h"

class SutherlandHodgman
{
public:
    SutherlandHodgman();

    std::vector<QPoint> GetIntersectedPoints(myPolygon inPoly, myPolygon clipBoundaryPoly);
};

#endif // SUTHERLANDHODGMAN_H
