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

void myPolygon::move(int dx, int dy) {
    std::vector<std::pair<int,int> > oldPoints = points;
    points.clear();
    lines.clear();

    for (auto point: oldPoints) {
        addPoint(point.first + dx, point.second + dy);
    }

    if (isFinished) {
        isFinished = false;
        setFinished();
    }
}

std::vector<PixelWithColor> myPolygon::getPixels() {
    std::vector<PixelWithColor> pixels;

    for (auto line: lines) {
        line.R = this->R;
        line.G = this->G;
        line.B = this->B;
        line.setThickness(thickness);
        std::vector<PixelWithColor> newPixels = line.getPixels();
        pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());
    }

    return pixels;
};

std::vector<PixelWithColor> myPolygon::getPixelsAliased() {

    std::vector<PixelWithColor> pixels;
    return pixels;
}

void myPolygon::setFinished() {
    if (isFinished)
        return;

    MyLine line = MyLine(points[getPointsSize()-1].first,
            points[getPointsSize()-1].second,
            points[0].first, points[0].second );

    lines.push_back(line);
    isFinished = true;
}

std::pair<int, int> myPolygon::compute2DCentroid() {
    std::pair<int, int> centroid = std::make_pair<int, int> (0, 0);

    double signedArea = 0.0;
    double x0 = 0.0; // Current vertex X
    double y0 = 0.0; // Current vertex Y
    double x1 = 0.0; // Next vertex X
    double y1 = 0.0; // Next vertex Y
    double a = 0.0;  // Partial signed area

    unsigned int vertexCount = points.size();
    for (unsigned int i = 0; i < vertexCount; ++i) {
        x0 = points[i].first;
        y0 = points[i].second;
        x1 = points[(i+1) % vertexCount].first;
        y1 = points[(i+1) % vertexCount].second;
        a = x0*y1 - x1*y0;
        signedArea += a;
        centroid.first += (x0 + x1)*a;
        centroid.second += (y0 + y1)*a;
    }

    signedArea *= 0.5;
    centroid.first /= (6.0*signedArea);
    centroid.second /= (6.0*signedArea);

    return centroid;
}
