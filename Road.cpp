#include "Road.h"
#include <math.h>

using namespace std;

Road::Road(Town* start, Town* finish)
{
    this->start = start;
    this->finish = finish;
}

Road::~Road()
{
    //dtor
}

void Road::computeDistance() {
    int x1 = this->start->GetX();
    int x2 = this->finish->GetX();
    int y1 = this->start->GetY();
    int y2 = this->finish->GetY();
    int dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
    this->distance = dist;
}
