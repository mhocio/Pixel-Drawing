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

    void addPoint(int x, int y);

    std::vector<std::pair<int,int> > points;
    std::vector<MyLine> lines;
};

#endif // MYPOLYGON_H
