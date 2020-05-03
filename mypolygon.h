#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include "ishape.h"
#include "myline.h"

class myPolygon : public IShape
{
public:
    myPolygon();
    myPolygon(std::vector<std::pair<int,int> > points);

    std::string ToString() const override;
    std::vector<PixelWithColor> getPixels() override;
    std::vector<PixelWithColor> getPixelsAliased() override;

    std::pair<int, int> compute2DCentroid();

    void addPoint(int x, int y);
    void move(int dx, int dy);

    void setFinished();

    std::vector<std::pair<int,int> > points;
    int getPointsSize() {
        return points.size();
    }

    std::vector<MyLine> lines;
    bool isFinished = false;
};

#endif // MYPOLYGON_H
