#include "mypizza.h"

myPizza::myPizza()
{
    this->R = 0;
    this->G = 0;
    this->B = 0;
}

myPizza::myPizza(std::pair<int, int> pA, std::pair<int, int> pB,std::pair<int, int> pC) {
    this->pA = pA;
    this->pB = pB;
    this->pC = pC;
}

json myPizza::getJsonFormat() {
    json item = getIShapeJson();
    item["shape"] = "pizza";
    item["pA"] = pA;
    item["pB"] = pB;
    item["pC"] = pC;

    return item;
}

std::string myPizza::ToString() const {
    std::string s;
    s += "A: " + std::to_string(pA.first) + " " + std::to_string(pA.second);
    s += " B: " + std::to_string(pB.first) + " " + std::to_string(pB.second);
    s += " C: " + std::to_string(pC.first) + " " + std::to_string(pC.second);
    return s;
}

float det(std::pair<int, int> A, std::pair<int, int> B, std::pair<int, int> C) {
    return A.first*B.second - A.first*C.second - A.second*B.first +
            A.second*C.first + B.first*C.second - B.second*C.first;
}

std::vector<PixelWithColor> myPizza::getPixelsAliased() {

    std::vector<PixelWithColor> pixels;
    return pixels;
}

std::vector<PixelWithColor> myPizza::getPixels() {
    std::vector<PixelWithColor> pixels;

    int radius = sqrt(pow(pB.first - pA.first, 2) + pow(pB.second - pA.second, 2) * 1.0);

    int x = radius;
    int y = 0;
    int err = 0;

    int X = pA.first;
    int Y = pA.second;

    //std::pair<int, int> pD;

    MyLine line = MyLine(X, Y, pB.first, pB.second);
    std::vector<PixelWithColor> newPixels = line.getPixels();
    pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());

// shortening the second line TODO
//    double d = std::sqrt((X-pC.first)*(X-pC.first) + (Y-pC.second)*(Y-pC.second));
//    double b = std::sqrt((1 - (dx*dx)/(dy*dy)) * d*d);
//    double a = a*dy/b;

    line = MyLine(X, Y, pC.first, pC.second);
    newPixels = line.getPixels();
    pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());

    while (x >= y) {
        std::vector<std::pair<int, int> > octants;
        octants.push_back(std::make_pair(X + x, Y + y));
        octants.push_back(std::make_pair(X + y, Y + x));
        octants.push_back(std::make_pair(X - y, Y + x));
        octants.push_back(std::make_pair(X - x, Y + y));
        octants.push_back(std::make_pair(X - x, Y - y));
        octants.push_back(std::make_pair(X - y, Y - x));
        octants.push_back(std::make_pair(X + y, Y - x));
        octants.push_back(std::make_pair(X + x, Y - y));

        for(auto p : octants) {
            if (det(pA, pB, pC) > 0) {
                if (det(pA, pB, p) > 0 && det(pA, pC, p) < 0) {
                    pixels.push_back(PixelWithColor(p.first, p.second, R, G, B));
                }
            } else {
                if (det(pA, pB, p) > 0 || det(pA, pC, p) < 0) {
                    pixels.push_back(PixelWithColor(p.first, p.second, R, G, B));
                }
            }
        }

        if (err <= 0) {
            y++;
            err += 2*y + 1;
        }

        if (err > 0) {
            x--;
            err -= 2*x + 1;
        }
    }

    return pixels;
}
