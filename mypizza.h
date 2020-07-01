#ifndef MYPIZZA_H
#define MYPIZZA_H

#include <cmath>
#include "ishape.h"
#include "myline.h"

class myPizza : public IShape
{
public:
    myPizza();
    myPizza(std::pair<int, int> pA, std::pair<int, int> pB,std::pair<int, int> pC);

    std::string ToString() const override;
    std::vector<PixelWithColor> getPixels() override;
    std::vector<PixelWithColor> getPixelsAliased() override;
    json getJsonFormat() override;

    std::pair<int, int> pA, pB, pC;
};

#endif // MYPIZZA_H
