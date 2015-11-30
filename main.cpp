#include <stdio.h>
#include <omp.h>
#include "WorldMap.h"
#include <time.h>

using namespace std;

int main() {




    WorldMap* worldMap = new WorldMap();

    //losuj zbior P punktow o licznosci N > 10(n+1), n - wymiar zadania
    worldMap->generateRandomPaths(200,0);

    //wyznacz punkt najlepszy i najgorszy
    int best = worldMap->findBestPath();
    int worst = worldMap->findWorstPath();



    int n = 10;

     vector<Town*> srodek;
     vector<Town*> odbicie;
     vector<int> sympleks;
     double x;
     struct drand48_data randBuffer;
     srand48_r(time(NULL), &randBuffer);
     vector<Town*> lepsza;
     int utknal = 0;
     int poprz_dlugosc = 0;
     int iteracje = 0;
     bool krecsie = true;
     int iii;
    //clock_t tStart = clock();
    double time = omp_get_wtime();
    #pragma omp parallel for private(iii, srodek, odbicie, sympleks, lepsza, utknal) num_threads(6) shared(worldMap, best, worst)
    for(iii = 0; iii < 1000; ++iii) {
        iteracje++;
        //if(iteracje % 100 == 0) cout<<"\niteracja "<<iteracje<<" watek: "<<omp_get_thread_num ();

        srodek.clear();
        odbicie.clear();
        sympleks.clear();
        lepsza.clear();


        //losuj ze zbioru P zbior n punktow i utworz n+1 wymiarowy sympleks
        //pierwszy wierzcholek - najlepszy punkt
        //# pragma omp atomic
        sympleks.push_back(best);
        //#pragma omp parallel for shared(worldMap, sympleks) private(x, randBuffer) num_threads(4)
        for(int i = 1; i < n+1; ++i) {
            drand48_r(&randBuffer, &x);
            x = (int)(x*1000.0)%worldMap->Getpaths().size();
            sympleks.push_back(x); //(worldMap->Getpaths())[x]
        }

        //wyznacz srodek sympleksu
        //#pragma omp parallel for shared(worldMap, srodek) num_threads(4)
        vector<Town*> currentBestPath = worldMap->GetbestPath();
        for(int i = 0; i < currentBestPath.size();++i) {
            int xSuma = (currentBestPath)[i]->GetX();
            int ySuma = (currentBestPath)[i]->GetY();
            for(int j = 1; j < n; ++j) {
                xSuma += (worldMap->Getpaths())[sympleks[j]][i]->GetX();
                ySuma += (worldMap->Getpaths())[sympleks[j]][i]->GetY();
            }
            xSuma /= n;
            ySuma /= n;
            //cout<<"\nwatek: "<<omp_get_thread_num ();
            srodek.push_back(new Town(xSuma, ySuma, 7+i, "srodek"));
        }

        //operacja odbicia ostatniego punktu sympleksu wzgledem srodka sympleksu
        //#pragma omp parallel for shared(worldMap, odbicie, srodek, sympleks) num_threads(4)
        for(int i = 0; i < srodek.size(); ++i) {
            int x = 2*srodek[i]->GetX() - ((worldMap->Getpaths())[sympleks[sympleks.size()-1]])[i]->GetX();
            int y = 2*srodek[i]->GetY() - ((worldMap->Getpaths())[sympleks[sympleks.size()-1]])[i]->GetY();
            Town* t = new Town(x,y,srodek[i]->Getid(),srodek[i]->Getname());
            odbicie.push_back(t);
        }

        //czy odbicie spelnia ograniczenia?
        //zaadaptowanie wyliczonej sciezki do istniejacych miast
        lepsza = worldMap->adjustPath(odbicie);

        //czy jest to punkt lepszy od ostatniego?
        if(worldMap->computePathLength(lepsza) < worldMap->computePathLength(worldMap->GetworstPath())) {
            #pragma opm critical
            worldMap->changePath(worst, lepsza);
            //wyznacz punkt najlepszy i najgorszy
            best = worldMap->findBestPath();
            worst = worldMap->findWorstPath();
            utknal = 0;
        }
        else {
            utknal++;
        }


        //jesli algorytm utknal ********************************************************
        if(utknal > 20) {
            cout<<"utknal! roznica: "<<worldMap->computePathLength(worldMap->GetworstPath()) - worldMap->computePathLength(worldMap->GetbestPath())<<endl;

            //cout<<"\nwygrala: ";
            //for(int i = 0; i < worldMap->Getpaths()[best].size(); ++i) cout<< worldMap->Getpaths()[best][i]->Getname()<<" ";
            //cout<<endl;

            iii = 10001;
        }
        poprz_dlugosc = worldMap->computePathLength(worldMap->GetworstPath());
        //jesli algorytm utknal ********************************************************


        //znaleziono rozwiazanie
        if(worldMap->computePathLength(worldMap->GetworstPath()) - worldMap->computePathLength(worldMap->GetbestPath()) == 0) {
            cout<<"roznica: "<<worldMap->computePathLength(worldMap->GetworstPath()) - worldMap->computePathLength(worldMap->GetbestPath())<<endl;

            //cout<<"\nwygrala: ";
            //for(int i = 0; i < worldMap->Getpaths()[best].size(); ++i) cout<< worldMap->Getpaths()[best][i]->Getname()<<" ";
            //cout<<endl;

            iii = 10001;
        }
        //worldMap->drawPath(lepsza, cv::Scalar(255,0,0));
        //cv::imshow("mapa", worldMap->worldMap);

        //cv::waitKey();
    }

    cout<<"\nwygrala: ";
    for(int i = 0; i < worldMap->Getpaths()[best].size(); ++i) cout<< worldMap->Getpaths()[best][i]->Getname()<<" ";
    cout<<endl;



    //cout<<"\n\ncalosc: "<<iteracje<<endl;
    cout<<"\n\nCZAS: "<<omp_get_wtime() - time<<" sekund\n\n";






}
