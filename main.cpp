#include <stdio.h>
#include <omp.h>
#include "WorldMap.h"
#include <time.h>

using namespace std;

int main() {




    WorldMap* worldMap = new WorldMap(10, 1000.0, 1000.0);

    //losuj zbior P punktow o licznosci N > 10(n+1), n - wymiar zadania
    worldMap->generateRandomPaths(200,0);

    //wyznacz punkt najlepszy i najgorszy
    int best = worldMap->findBestPath();
    int worst = worldMap->findWorstPath();



    int n = 10;

     vector<Town*> srodek;
     vector<Town*> odbicie;
     vector<Town*> ekspansja;
     vector<Town*> kontrakcja;
     vector<Town*> redukcja;
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
     bool znaleziono = false;
    //clock_t tStart = clock();


    double time = omp_get_wtime();
    //#pragma omp parallel for private(iii, srodek, odbicie, sympleks, lepsza, utknal, ekspansja, kontrakcja, redukcja) num_threads(2) shared(worldMap, best, worst, znaleziono)
    for(iii = 0; iii < 10000000; ++iii) {
        if(znaleziono) {
            iii = 10000001;
            continue;
        }
        iteracje++;
        //if(iteracje % 100 == 0) cout<<"\niteracja "<<iteracje<<" watek: "<<omp_get_thread_num ();

        srodek.clear();
        odbicie.clear();
        sympleks.clear();
        lepsza.clear();
        ekspansja.clear();
        kontrakcja.clear();
        redukcja.clear();


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

        if(iii%10 == 0) {
            for(int i = 0; i < odbicie.size(); ++i) {
                int x = odbicie[i]->GetX() + 1.5 * (odbicie[i]->GetX() - srodek[i]->GetX());
                int y = odbicie[i]->GetY() + 1.5 * (odbicie[i]->GetY() - srodek[i]->GetY());
                Town* t = new Town(x,y,odbicie[i]->Getid(),odbicie[i]->Getname());
                ekspansja.push_back(t);
            }
            lepsza = worldMap->adjustPath(ekspansja);
            //czy jest to punkt lepszy od ostatniego?
            if(worldMap->computePathLength(lepsza) < worldMap->computePathLength(worldMap->GetworstPath())) {
                #pragma opm critical
                worldMap->changePath(worst, lepsza);
                //wyznacz punkt najlepszy i najgorszy
                #pragma opm critical
                best = worldMap->findBestPath();
                #pragma opm critical
                worst = worldMap->findWorstPath();
                utknal = 0;
            }
            else {
                utknal++;
            }
        }
        else if(iii%15 == 0) {
            for(int i = 0; i < srodek.size(); ++i) {
                int x = srodek[i]->GetX() + 0.5 * (worldMap->Getpaths()[worst][i]->GetX() - srodek[i]->GetX());
                int y = srodek[i]->GetY() + 0.5 * (worldMap->Getpaths()[worst][i]->GetY() - srodek[i]->GetY());
                Town* t = new Town(x,y,srodek[i]->Getid(),srodek[i]->Getname());
                kontrakcja.push_back(t);
            }
            lepsza = worldMap->adjustPath(kontrakcja);
            //czy jest to punkt lepszy od ostatniego?
            if(worldMap->computePathLength(lepsza) < worldMap->computePathLength(worldMap->GetworstPath())) {
                #pragma opm critical
                worldMap->changePath(worst, lepsza);
                //wyznacz punkt najlepszy i najgorszy
                #pragma opm critical
                best = worldMap->findBestPath();
                #pragma opm critical
                worst = worldMap->findWorstPath();
                utknal = 0;
            }
            else {
                utknal++;
            }
        }
        else if(iii%20 == 0) {

            for(int i = 0; i < worldMap->Getpaths().size();++i) {
                for(int j = 0; j < srodek.size(); ++j) {
                    int bestX = worldMap->Getpaths()[best][j]->GetX();
                    int bestY = worldMap->Getpaths()[best][j]->GetY();
                    int x = (bestX + worldMap->Getpaths()[i][j]->GetX())/2;
                    int y = (bestY + worldMap->Getpaths()[i][j]->GetY())/2;
                    Town* t = new Town(x,y,worldMap->Getpaths()[i][j]->Getid(),worldMap->Getpaths()[i][j]->Getname());
                    redukcja.push_back(t);
                }
                #pragma opm critical
                worldMap->changePath(i, redukcja);
                redukcja.clear();
            }
        }
        else {
            lepsza = worldMap->adjustPath(odbicie);
            //czy jest to punkt lepszy od ostatniego?
            if(worldMap->computePathLength(lepsza) < worldMap->computePathLength(worldMap->GetworstPath())) {
                #pragma opm critical
                worldMap->changePath(worst, lepsza);
                //wyznacz punkt najlepszy i najgorszy
                #pragma opm critical
                best = worldMap->findBestPath();
                #pragma opm critical
                worst = worldMap->findWorstPath();
                utknal = 0;
            }
            else {
                utknal++;
            }
        }

        //czy odbicie spelnia ograniczenia?
        //zaadaptowanie wyliczonej sciezki do istniejacych miast





        //jesli algorytm utknal ********************************************************
        if(utknal > 1000) {
            //cout<<"utknal! roznica: "<<worldMap->computePathLength(worldMap->GetworstPath()) - worldMap->computePathLength(worldMap->GetbestPath())<<endl;

            //cout<<"\nwygrala: ";
            //for(int i = 0; i < worldMap->Getpaths()[best].size(); ++i) cout<< worldMap->Getpaths()[best][i]->Getname()<<" ";
            //cout<<endl;

            iii = 10000001;
        }
        poprz_dlugosc = worldMap->computePathLength(worldMap->GetworstPath());
        //jesli algorytm utknal ********************************************************


        //znaleziono rozwiazanie
        if(worldMap->computePathLength(worldMap->Getpaths()[worst]) - worldMap->computePathLength(worldMap->Getpaths()[best]) == 0) {
            //cout<<"roznica: "<<worldMap->computePathLength(worldMap->Getpaths()[worst]) - worldMap->computePathLength(worldMap->Getpaths()[best])<<endl;

            //cout<<"\nwygrala: ";
            //for(int i = 0; i < worldMap->Getpaths()[best].size(); ++i) cout<< worldMap->Getpaths()[best][i]->Getname()<<" ";
            //cout<<endl;

            iii = 10000001;
            znaleziono = true;

        }
        //worldMap->drawPath(lepsza, cv::Scalar(255,0,0));
        //cv::imshow("mapa", worldMap->worldMap);

        //cv::waitKey();
    }

    //cout<<"\nwygrala: ";
    //for(int i = 0; i < worldMap->GetbestPath().size(); ++i) cout<< worldMap->GetbestPath()[i]->Getname()<<" ";
    //cout<<endl;
    //cout<<"\n\ncalosc: "<<iteracje<<endl;
    //cout<<"\n\nCZAS: "<<omp_get_wtime() - time<<" sekund\n\n";
    cout<<omp_get_wtime() - time<<" ";

    /*worldMap->showMap();
    worldMap->drawPath(worldMap->GetbestPath(), cv::Scalar(0,255,0),5);
    cv::imshow("mapa", worldMap->worldMap);
    cv::waitKey();*/


}
