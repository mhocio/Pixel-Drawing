#include "ishape.h"

bool operator== (const IShape &c1, const IShape &c2)
{
    return c1.ToString() == c2.ToString();
}

bool operator!= (const IShape &c1, const IShape &c2)
{
    return !(c1== c2);
}

std::string IShape::ToString() const {
    return "";
}

void IShape::setColor(QColor color) {
    R = qRed(color.rgb());
    G = qGreen(color.rgb());
    B = qBlue(color.rgb());
}

json IShape::getIShapeJson() {
    json item;
    item["color"] = {R, G, B};
    item["thickness"] = thickness;

    return item;
}

