#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "Road.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <omp.h>

using namespace std;

class WorldMap
{
    public:
        WorldMap();
        virtual ~WorldMap();
        vector<Road*> Getroads() { return roads; }
        void Setroads(vector<Road*> val) { roads = val; }
        vector<Town*> Gettowns() { return towns; }
        void Settowns(vector<Town*> val) { towns = val; }
        void printTowns();
        void printRoads();
        void showMap();
        vector<int> generateRandomPath(int display);
        //void generateRandomPaths(int iterations, int display);
        void generateRandomPaths(int iterations, int display);
        //int computePathLength(vector<int> path);
        int computePathLength(vector<Town*> path);
        int findBestPath();
        int findWorstPath();
        vector<vector<Town*> > Getpaths() { return paths; }
        vector<Town*> GetbestPath() { return bestPath; }
        vector<Town*> GetworstPath() { return worstPath; }
        vector<Town*> adjustPath(vector<Town*> path);
        void drawPath(vector<Town*> path, cv::Scalar color, int thickness);
        void changePath(int index, vector<Town*> path);

        cv::Mat_<cv::Vec3b> worldMap;
    protected:
    private:
        //int roadLength(int t1, int t2);
        int roadLength(Town* t1, Town* t2);
        vector<Road*> roads;
        vector<Town*> towns;
        //vector<vector<int> > paths;
        vector<vector<Town*> > paths;
        vector<Town*> bestPath;
        vector<Town*> worstPath;
};

#endif // WORLDMAP_H
