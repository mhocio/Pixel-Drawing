#ifndef AETNODE_H
#define AETNODE_H

#include <QPoint>

class AETnode
{
public:
    AETnode(QPoint p1, QPoint p2);
    double yMax;
    double yMin;

    double xMin;
    double xMax;

    double m_inverse;

    bool isSlopeZero() const {
        return (yMax - yMin == 0 ? true : false);
    }

    bool operator ==(const AETnode &b) const;
};

#endif // AETNODE_H
