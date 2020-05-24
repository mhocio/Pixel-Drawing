#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include <cmath>
#include <algorithm>
#include "ishape.h"
#include "myline.h"
#include "aet.h"

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

    // filling the polygon
    bool isFilled = false;
    QColor fillingColor;
    void setFilled(QColor c) {
        isFilled = true;
        fillingColor = c;
    }

    std::vector<PixelWithColor> getFillingPixels() {
        std::vector<PixelWithColor> ret;

        if (!isFilled)
            return ret;

        AET aet = AET();
        int fillR = qRed(fillingColor.rgb());
        int fillG = qGreen(fillingColor.rgb());
        int fillB = qBlue(fillingColor.rgb());

        // create indices
        int i = 0;
        std::vector<std::pair<int, int> > ind;
        for (auto point: points)
            ind.push_back(std::make_pair(point.second, i++));
        std::sort(ind.begin(), ind.end(), [](const std::pair<int, int>& el1, const std::pair<int, int>& el2)
                                {return el1.first < el2.first;});

        std::vector<int> indices;
        for (auto p: ind)
            indices.push_back(p.second);

        int k = 0;
        i = indices[k];
        int N = points.size();
        int y, yMin, yMax;
        y = yMin = points[indices[0]].second;
        yMax = points[indices[N-1]].second;

        while (y < yMax) {
            while (points[i].second == y) {
                // wraping indies in polygon
                int prev = i-1, next = i + 1;
                if (prev == -1)
                    prev = N-1;
                if (next == N)
                    next = 0;

                if (points[prev].second > points[i].second)
                    aet.add(AETnode(QPoint(points[i].first, points[i].second),
                                                QPoint(points[prev].first, points[prev].second)));
                if (points[next].second > points[i].second)
                    aet.add(AETnode(QPoint(points[i].first, points[i].second),
                                                QPoint(points[next].first, points[next].second)));
                ++k;
                i = indices[k];
            }
            aet.nodes.sort( [](const AETnode& a, const AETnode& b) {
                    return a.xMin < b.xMin; });

            qDebug() << "y: " << y;
            aet.print();

            // fill pixels between pairs of intersections
            unsigned int bucketSize = aet.nodes.size();
            std::vector<double> xVals;
            std::list<AETnode>::iterator it;
            for (it = aet.nodes.begin(); it != aet.nodes.end(); ++it)
                xVals.push_back(it->xMin);

            for (unsigned int bucketIndex = 0; bucketIndex < bucketSize - 1; bucketIndex++) {
                if (bucketIndex % 2 == 1)
                    continue;
                int beginX = ceil(xVals[bucketIndex]);
                int endX = floor(xVals[bucketIndex + 1]);
                for (int xx = beginX; xx <= endX; xx++)
                    ret.push_back(PixelWithColor(xx, y, fillR, fillG, fillB));
            }

            ++y;
            aet.nodes.remove_if([y](AETnode &a){return a.yMax == y;});
            for (auto &edge: aet.nodes)
                edge.xMin += edge.m_inverse;
        }

        return ret;
    }
};

#endif // MYPOLYGON_H
