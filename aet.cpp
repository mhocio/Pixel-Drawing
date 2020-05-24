#include "aet.h"

AET::AET(std::vector<std::pair<int,int> > points)
{
    this->points = points;

    for (unsigned int i = 0; i < points.size() - 1; i++) {
        QPoint p1 = QPoint(points[i].first, points[i].second);
        QPoint p2 = QPoint(points[i+1].first, points[i+1].second);
        this->add(AETnode(p1, p2));
    }
}

AET::AET() {}

void AET::add(const AETnode node) {
    if (!node.isSlopeZero()
            ) {
        nodes.push_back(node);
    }
}

void AET::print() {
    std::list<AETnode>::iterator it;
    qDebug() << "AET:";
    for (it = nodes.begin(); it != nodes.end(); ++it){
        qDebug() << "Ymax: " << it->yMax << " Xmin: " << it->xMin << " 1/m: " << it->m_inverse;
    }
    qDebug() << "";
}
