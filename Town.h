#ifndef TOWN_H
#define TOWN_H

#include <string>

using namespace std;

class Town
{
    public:
        Town();
        Town(int X, int Y, int id, string name);
        virtual ~Town();
        int GetX() { return X; }
        void SetX(int val) { X = val; }
        int GetY() { return Y; }
        void SetY(int val) { Y = val; }
        int Getid() { return id; }
        void Setid(int val) { id = val; }
        string Getname() { return name; }
        void Setname(string val) { name = val; }
    protected:
    private:
        int X;
        int Y;
        int id;
        string name;
};

#endif // TOWN_H
