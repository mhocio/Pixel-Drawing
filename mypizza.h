#ifndef MYPIZZA_H
#define MYPIZZA_H

#include <cmath>
#include "ishape.h"
#include "myline.h"

class myPizza : public IShape
{
public:
    myPizza();

    std::string ToString() const override;
    std::vector<PixelWithColor> getPixels() override;
    std::vector<PixelWithColor> getPixelsAliased() override;

    std::pair<int, int> pA, pB, pC;
};

#endif // MYPIZZA_H
