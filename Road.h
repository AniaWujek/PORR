#ifndef ROAD_H
#define ROAD_H

#include "Town.h"

using namespace std;

class Road
{
    public:
        Road(Town* start, Town* finish);
        virtual ~Road();
        Town* Getstart() { return start; }
        void Setstart(Town* val) { start = val; }
        Town* Getfinish() { return finish; }
        void Setfinish(Town* val) { finish = val; }
        int Getdistance() { return distance; }
        void Setdistance(int val) { distance = val; }
        void computeDistance();
    protected:
    private:
        Town* start;
        Town* finish;
        int distance;
};

#endif // ROAD_H
