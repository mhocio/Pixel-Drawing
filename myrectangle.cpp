#include "myrectangle.h"

myRectangle::myRectangle() {
    this->R = 0;
    this->G = 0;
    this->B = 0;
}

json myRectangle::getJsonFormat() {
    json item = myPolygon::getJsonFormat();
    item["shape"] = "rectangle";
    item["firstPoint"] = firstPoint;
    item["secondPoint"] = secondPoint;

    return item;
}

myRectangle::myRectangle(std::pair<int, int> A, std::pair<int, int> B) {
    myRectangle();

    int minX = std::min(A.first, B.first);
    int maxX = std::max(A.first, B.first);

    int minY = std::min(A.second, B.second);
    int maxY = std::max(A.second, B.second);

    addPoint(minX, minY);
    addPoint(minX, maxY);
    addPoint(maxX, maxY);
    addPoint(maxX, minY);

    firstPoint = std::make_pair(minX, minY);
    secondPoint = std::make_pair(maxX, maxY);

    setFinished();
}
