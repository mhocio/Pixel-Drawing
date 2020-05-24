#include "aetnode.h"

AETnode::AETnode(QPoint p1, QPoint p2)
{
    if (p1.y() < p2.y()) {
        xMin = p1.x();
        xMax = p2.x();
        yMin = p1.y();
        yMax = p2.y();
    } else if (p1.y() > p2.y()) {
        xMin = p2.x();
        xMax = p1.x();
        yMin = p2.y();
        yMax = p1.y();
    } else {
        yMin = yMax = p1.y();
        xMin = (p1.x() <= p2.x()) ? p1.x() : p2.x();
        xMax = (p1.x() <= p2.x()) ? p2.x() : p1.x();
    }

    m_inverse = (xMax - xMin) / (yMax - yMin);
}

bool AETnode::operator ==(const AETnode &b) const {
    if (this->yMax != b.yMax)
        return false;
    if (this->xMin != b.xMin)
        return false;
    if (this->m_inverse != b.m_inverse)
        return false;
    return true;
}
