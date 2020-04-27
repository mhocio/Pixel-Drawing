#ifndef MYPIZZA_H
#define MYPIZZA_H

#include "ishape.h"

class myPizza : public IShape
{
public:
    myPizza();

    std::string ToString() const override;
    std::vector<PixelWithColor> getPixels() override;

    std::pair<int, int> A, B, C;
};

#endif // MYPIZZA_H
