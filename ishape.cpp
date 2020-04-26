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
