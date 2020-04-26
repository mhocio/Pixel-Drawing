#include "mypolygon.h"

myPolygon::myPolygon()
{
    this->R = 0;
    this->G = 0;
    this->B = 0;
}

myPolygon::myPolygon(std::vector<std::pair<int,int> > points) {
    this->R = 0;
    this->G = 0;
    this->B = 0;

    for (std::pair<int,int> point: points)
        addPoint(point.first, point.second);
}

std::string myPolygon::ToString() const {
    std::string s;
    for (std::pair<int, int> point: points) {
        s += "(" + std::to_string(point.first) + ", " +
                std::to_string(point.second) + "), ";
    }
    return s;
}

void myPolygon::addPoint(int x, int y) {
    points.push_back(std::make_pair(x,y));

    if (this->points.size() > 1) {
        MyLine newLine = MyLine(points[points.size()-2].first,
                points[points.size()-2].second, x, y);

        lines.push_back(newLine);
    }
}

std::vector<PixelWithColor> myPolygon::getPixels() {
    std::vector<PixelWithColor> pixels;

    for (auto line: lines) {
        line.R = this->R;
        line.G = this->G;
        line.B = this->B;
        std::vector<PixelWithColor> newPixels = line.getPixels();
        pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());
    }

    return pixels;
};
