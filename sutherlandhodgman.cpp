#include "sutherlandhodgman.h"

SutherlandHodgman::SutherlandHodgman()
{

}

struct Point {
   int x, y;
};

struct line {
   Point p1, p2;
};

bool onLine(line l1, Point p) {        //check whether p is on the line or not
   if(p.x <= std::max(l1.p1.x, l1.p2.x) && p.x <= std::min(l1.p1.x, l1.p2.x) &&
      (p.y <= std::max(l1.p1.y, l1.p2.y) && p.y <= std::min(l1.p1.y, l1.p2.y)))
         return true;

   return false;
}

int direction(Point a, Point b, Point c) {
   int val = (b.y-a.y)*(c.x-b.x)-(b.x-a.x)*(c.y-b.y);
   if (val == 0)
      return 0;           //colinear
   else if (val < 0)
      return 2;          //anti-clockwise direction
    return 1;          //clockwise direction
}

bool isIntersect(line l1, line l2) {
   //four direction for two lines and points of other line
   int dir1 = direction(l1.p1, l1.p2, l2.p1);
   int dir2 = direction(l1.p1, l1.p2, l2.p2);
   int dir3 = direction(l2.p1, l2.p2, l1.p1);
   int dir4 = direction(l2.p1, l2.p2, l1.p2);

   if(dir1 != dir2 && dir3 != dir4)
      return true;           //they are intersecting
   if(dir1==0 && onLine(l1, l2.p1))        //when p2 of line2 are on the line1
      return true;
   if(dir2==0 && onLine(l1, l2.p2))         //when p1 of line2 are on the line1
      return true;
   if(dir3==0 && onLine(l2, l1.p1))       //when p2 of line1 are on the line2
      return true;
   if(dir4==0 && onLine(l2, l1.p2)) //when p1 of line1 are on the line2
      return true;
   return false;
}

bool linesIntersect(Point& p1, Point& p2, Point& q1, Point& q2) {
    return (((q1.x-p1.x)*(p2.y-p1.y) - (q1.y-p1.y)*(p2.x-p1.x))
            * ((q2.x-p1.x)*(p2.y-p1.y) - (q2.y-p1.y)*(p2.x-p1.x)) < 0)
            &&
           (((p1.x-q1.x)*(q2.y-q1.y) - (p1.y-q1.y)*(q2.x-q1.x))
            * ((p2.x-q1.x)*(q2.y-q1.y) - (p2.y-q1.y)*(q2.x-q1.x)) < 0);
}

bool isInside(QPoint point, myPolygon poly) {
    int N = poly.points.size();
    if (N < 3)
        return false;

    Point p;
    p.x = point.x();
    p.y = point.y();

    line exline = {p, {9999, p.y}};  //create a point at infinity, y is same as point p
    int count = 0;
    int i = 0;
    do {
        Point p1, p2;
        p1.x = poly.points[i].first;
        p1.y = poly.points[i].second;
        p2.x = poly.points[(i+1) % N].first;
        p2.y = poly.points[(i+1) % N].second;
        line side = {p1, p2};  //forming a line from two consecutive points of poly
        if (isIntersect(side, exline)) {  //if side is intersects exline
            if (direction(side.p1, p, side.p2) == 0)
                return onLine(side, p);
            count++;
        }
        i = (i+1) % N;
    } while(i != 0);

   return count&1;
}

QPoint intersect(QPoint A, QPoint B, myPolygon poly) {
    // Line AB represented as a1x + b1y = c1
    double a1 = B.y() - A.y();
    double b1 = A.x() - B.x();
    double c1 = a1*(A.x()) + b1*(A.y());

    for (MyLine polyLine: poly.lines) {
        // Line CD represented as a2x + b2y = c2
        line l1 = {Point{A.x(), A.y()}, Point{B.x(), B.y()}};
        line l2 = {Point{polyLine.x1, polyLine.y1},
                   Point{polyLine.x2, polyLine.y2}};

        Point p1 = Point{A.x(), A.y()};
        Point p2 = Point{B.x(), B.y()};
        Point p3 = Point{polyLine.x1, polyLine.y1};
        Point p4 = Point{polyLine.x2, polyLine.y2};

        if (!linesIntersect(p1, p2, p3, p4))
            continue;

        double a2 = polyLine.y2 - polyLine.y1;
        double b2 = polyLine.x1 - polyLine.x2;
        double c2 = a2*(polyLine.x1)+ b2*(polyLine.y1);

        double determinant = a1*b2 - a2*b1;

        if (determinant != 0) {
            double x = (b2*c1 - b1*c2) / determinant;
            double y = (a1*c2 - a2*c1) / determinant;
            return QPoint(x, y);
        }
    }
    return QPoint(-1, -1);
}

std::vector<QPoint> SutherlandHodgman::GetIntersectedPoints(myPolygon inPoly, myPolygon clipBoundaryPoly) {
    std::vector<QPoint> ret;
    QPoint p, i;
    std::pair<int, int> sPair = inPoly.points[inPoly.points.size() - 1];
    QPoint s = QPoint(sPair.first, sPair.second);

    for (unsigned int j = 0; j < inPoly.points.size(); ++j) {
        std::pair<int, int> pPair = inPoly.points[j];
        p = QPoint(pPair.first, pPair.second);

        if (isInside(p, clipBoundaryPoly)) {
            qDebug() << "YES1";
            if (isInside(s, clipBoundaryPoly))
                ret.push_back(p); //Case1
            else {
                i = intersect(s, p, clipBoundaryPoly);
                ret.push_back(i); //Case 4
                ret.push_back(p);
            }
        } else if (isInside(s, clipBoundaryPoly)) {
            qDebug() << "YES2";
            i = intersect(s, p, clipBoundaryPoly);
            ret.push_back(i); //Case 2
        }
        s = p;
    }
    return ret;
}
