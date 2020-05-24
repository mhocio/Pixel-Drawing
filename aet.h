#ifndef AET_H
#define AET_H

#include "aetnode.h"
#include <iostream>
#include <list>
#include <vector>
#include <qdebug.h>

class AET
{
public:
    AET(std::vector<std::pair<int,int> > points);
    AET();

    int minY;
    int maxY;

    std::vector<std::pair<int,int> > points;

    void add(const AETnode node);

    std::list<AETnode> nodes;

    void print();
};

#endif // AET_H
