#include "mypolygon.h"

myPolygon::myPolygon()
{
    this->R = 0;
    this->G = 0;
    this->B = 0;
}

json myPolygon::getJsonFormat() {
    json item = getIShapeJson();
    item["shape"] = "polygon";
    for (auto point : points)
        item["points"].push_back({point.first, point.second});

    item["isFilled"] = isFilled;

    item["isFilledWithColor"] = isFilledWithColor;
    item["fillingColor"] = {fillingColor.red(), fillingColor.green(), fillingColor.blue()};

    item["isFilledWithImage"] = isFilledWithImage;

    /*if (isFilledWithImage) {
        QByteArray ba = QByteArray::fromRawData((const char*)fillingImage.bits(), fillingImage.byteCount());
        qDebug() << "ba size:" << ba.size() << "\n";

        char *data = ba.data();
        while (*data) {
            //qDebug() << "[" << *data << "]" << Qt::endl;
            item["fillingImage"].push_back(data);
            ++data;
        }
    }*/

    return item;
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

void myPolygon::generateLines() {
    points.clear();
    lines.clear();
    for (auto point: tmpPoints)
        addPoint(point.first, point.second);

    isFinished = false;
    setFinished();
}

float distance2(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

void myPolygon::moveVertex(int newX, int newY) {
    if (points.size() < 1)
        return;

    qDebug() << "moving vertex";

    std::pair<int,int>* closestPoint = &(points[0]);
    float minDistance = distance2(newX, newY, closestPoint->first, closestPoint->second);

    for (std::pair<int,int>& point: points) {
        if (distance2(newX, newY, point.first, point.second) < minDistance) {
            minDistance = distance2(newX, newY, point.first, point.second);
            closestPoint = &point;
        }
    }

    closestPoint->first = newX;
    closestPoint->second = newY;

    tmpPoints.clear();
    tmpPoints = points;
    generateLines();
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

    for (auto line: lines) {
        line.R = this->R;
        line.G = this->G;
        line.B = this->B;
        line.setThickness(thickness);
        std::vector<PixelWithColor> newPixels = line.getPixelsAliased();
        pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());
    }

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

float CrossProductLength(float Ax, float Ay,
    float Bx, float By, float Cx, float Cy)
{
    // Get the vectors' coordinates.
    float BAx = Ax - Bx;
    float BAy = Ay - By;
    float BCx = Cx - Bx;
    float BCy = Cy - By;

    // Calculate the Z coordinate of the cross product.
    return (BAx * BCy - BAy * BCx);
}

// http://csharphelper.com/blog/2014/07/determine-whether-a-polygon-is-convex-in-c/
bool myPolygon::isConvex() {
    bool got_negative = false;
    bool got_positive = false;
    int num_points = points.size();
    int B, C;
    for (int A = 0; A < num_points; A++) {
        B = (A + 1) % num_points;
        C = (B + 1) % num_points;

        float cross_product =
            CrossProductLength(
                points[A].first, points[A].second,
                points[B].first, points[B].second,
                points[C].first, points[C].second);
        if (cross_product < 0) {
            got_negative = true;
        } else if (cross_product > 0) {
            got_positive = true;
        }
        if (got_negative && got_positive)
            return false;
    }

    return true;
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
