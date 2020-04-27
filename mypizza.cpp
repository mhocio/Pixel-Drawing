#include "mypizza.h"

myPizza::myPizza()
{

}

std::string myPizza::ToString() const {
    std::string s;
    s += "A: " + std::to_string(A.first) + " " + std::to_string(A.second);
    s += " B: " + std::to_string(B.first) + " " + std::to_string(B.second);
    s += " C: " + std::to_string(C.first) + " " + std::to_string(C.second);
    return s;
}

std::vector<PixelWithColor> myPizza::getPixels() {

}
