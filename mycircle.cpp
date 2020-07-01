#include "mycircle.h"

MyCircle::MyCircle()
{
    this->R = 0;
    this->G = 0;
    this->B = 0;
}

MyCircle::MyCircle(int X, int Y, int radius) {
    this->R = 0;
    this->G = 0;
    this->B = 0;

    this->X = X;
    this->Y = Y;
    this->radius = radius;
}

std::string MyCircle::ToString() const {
    std::string s = "Circle x: " + std::to_string(X) + " y: " +
            std::to_string(Y) + " radius: " + std::to_string(radius);

    return s;
}

json MyCircle::getJsonFormat() {
    json item = getIShapeJson();
    item["shape"] = "circle";
    item["X"] = X;
    item["Y"] = Y;
    item["radius"] = radius;

    return item;
}
