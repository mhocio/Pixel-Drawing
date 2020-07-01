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

std::string MyLine::ToString() const {
    std::string s = "Line (" + std::to_string(x1) + ", " +
            std::to_string(y1) + "), (" +
            std::to_string(x2) + ", " + std::to_string(y2) + ")";

    return s;
}

json MyLine::getJsonFormat() {
    json item = getIShapeJson();
    item["shape"] = "line";
    item["x1"] = x1;
    item["y1"] = y1;
    item["x2"] = x2;
    item["y2"] = y2;

    return item;
}
