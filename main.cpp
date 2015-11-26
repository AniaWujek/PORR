#include <stdio.h>
#include <omp.h>
#include "WorldMap.h"

using namespace std;

int main() {


    WorldMap* worldMap = new WorldMap();
//    worldMap->printTowns();
//    worldMap->printRoads();
    worldMap->generateRandomPaths(200,0);

    cout<<"best: "<<worldMap->findBestPath()<<endl;
    cout<<"worst: "<<worldMap->findWorstPath()<<endl;

    int n = 10;
    vector<int> sympleks;
    double x;
    struct drand48_data randBuffer;
    srand48_r(time(NULL), &randBuffer);
    vector<Town*> srodek;
    vector<Town*> odbicie;
    vector<Town*> lepsza;
    cout<<"roznica: "<<worldMap->computePathLength(worldMap->GetbestPath()) - worldMap->computePathLength(worldMap->GetworstPath())<<endl;
    bool krecsie = true;
    int utknal = 0;
    int poprz_dlugosc = 0;
    while(krecsie) {
        //a = false;
        cout<<"roznica: "<<worldMap->computePathLength(worldMap->GetworstPath()) - worldMap->computePathLength(worldMap->GetbestPath())<<endl;
        int best = worldMap->findBestPath();
        int worst = worldMap->findWorstPath();
        sympleks.clear();
        srodek.clear();
        odbicie.clear();
        lepsza.clear();
        sympleks.push_back(worldMap->findBestPath());

        for(int i = 0; i < n; ++i) {
            drand48_r(&randBuffer, &x);
            x = (int)(x*1000.0)%n;
            sympleks.push_back(x); //(worldMap->Getpaths())[x]
        }

        for(int i = 0; i < (worldMap->Getpaths())[0].size();++i) {
            int xSuma = (worldMap->GetbestPath())[i]->GetX();
            int ySuma = (worldMap->GetbestPath())[i]->GetY();
            for(int j = 1; j < n; ++j) {
                xSuma += (worldMap->Getpaths())[sympleks[j]][i]->GetX();
                ySuma += (worldMap->Getpaths())[sympleks[j]][i]->GetY();
            }
            xSuma /= n;
            ySuma /= n;
            srodek.push_back(new Town(xSuma, ySuma, 7+i, "srodek"));
        }

        for(int i = 0; i < srodek.size(); ++i) {
            int x = 2*srodek[i]->GetX() - ((worldMap->Getpaths())[sympleks[sympleks.size()-1]])[i]->GetX();
            int y = 2*srodek[i]->GetY() - ((worldMap->Getpaths())[sympleks[sympleks.size()-1]])[i]->GetY();
            Town* t = new Town(x,y,srodek[i]->Getid(),srodek[i]->Getname());
            odbicie.push_back(t);
        }


        //worldMap->showMap();
        //worldMap->drawPath(odbicie, cv::Scalar(0,0,255));
        lepsza = worldMap->adjustPath(odbicie);

        if(worldMap->computePathLength(lepsza) < worldMap->computePathLength(worldMap->GetworstPath())) {
            worldMap->changePath(worst, lepsza);
            cout<<" +";
            utknal = 0;
        }
        else {
            cout<<" -";
            utknal++;
        }

        if(utknal > 50) {
            cout<<"utknal! roznica: "<<worldMap->computePathLength(worldMap->GetworstPath()) - worldMap->computePathLength(worldMap->GetbestPath())<<endl;
            worldMap->showMap();
            worldMap->drawPath(odbicie, cv::Scalar(0,0,255),2);
            worldMap->drawPath(lepsza, cv::Scalar(255,0,0),2);
            worldMap->drawPath(worldMap->GetbestPath(), cv::Scalar(0,255,0),5);
            cv::imshow("mapa", worldMap->worldMap);
            cv::waitKey();
            break;
        }
        poprz_dlugosc = worldMap->computePathLength(worldMap->GetworstPath());

        if(worldMap->computePathLength(worldMap->GetworstPath()) - worldMap->computePathLength(worldMap->GetbestPath()) < 10) {
            cout<<"roznica: "<<worldMap->computePathLength(worldMap->GetworstPath()) - worldMap->computePathLength(worldMap->GetbestPath())<<endl;
            worldMap->showMap();
            worldMap->drawPath(odbicie, cv::Scalar(0,0,255),2);
            worldMap->drawPath(lepsza, cv::Scalar(255,0,0),2);
            worldMap->drawPath(worldMap->GetbestPath(), cv::Scalar(0,255,0),5);
            cv::imshow("mapa", worldMap->worldMap);
            cv::waitKey();
            break;
        }
        //worldMap->drawPath(lepsza, cv::Scalar(255,0,0));
        //cv::imshow("mapa", worldMap->worldMap);

        /*if(worldMap->computePathLength(lepsza) < worldMap->computePathLength(worldMap->GetworstPath()))
            cout<<"lepsza!\n";
        if(worldMap->computePathLength(lepsza) < worldMap->computePathLength(worldMap->GetbestPath()))
            cout<<" jeszcze lepsza!\n";*/

        //cv::waitKey();
    }



}
