#include "Town.h"

using namespace std;

Town::Town(int X, int Y, int id, string name)
{
    this->X = X;
    this->Y = Y;
    this->id = id;
    this->name = name;
}

Town::~Town()
{
    //dtor
}
