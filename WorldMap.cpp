#include "WorldMap.h"

using namespace std;

WorldMap::WorldMap()
{
    Town* t1 = new Town(750,550,1,"t1");
	Town* t2 = new Town(550,250,2,"t2");
	Town* t3 = new Town(370,650,3,"t3");
	Town* t4 = new Town(120,140,4,"t4");
	Town* t5 = new Town(220,750,5,"t5");
	Town* t6 = new Town(180,250,6,"t6");
	Town* t7 = new Town(550,360,7,"t7");
	towns.push_back(t1);
	towns.push_back(t2);
	towns.push_back(t3);
	towns.push_back(t4);
	towns.push_back(t5);
	towns.push_back(t6);
	towns.push_back(t7);
	for(int i = 0; i < towns.size()-1;++i) {
        for(int j = i+1; j < towns.size(); ++j) {
            Road* r = new Road(towns[i],towns[j]);
            r->computeDistance();
            roads.push_back(r);
        }

	}
	int sizeY = 800;
	int sizeX = 800;
	int boxSize = 80;
	int rows = sizeY;
	int columns = sizeX;
	cv::Mat m(rows, columns, CV_8UC3, cv::Scalar(255,255,255));
	worldMap = m;
}

WorldMap::~WorldMap()
{
    //dtor
}
void WorldMap::printTowns() {
    for(int i = 0; i < towns.size(); ++i) {
        cout<<towns[i]->Getname()<<"\n";
    }
}
void WorldMap::printRoads() {
    for(int i = 0; i < roads.size(); ++i) {
        cout<<roads[i]->Getstart()->Getname()<<" "<<roads[i]->Getfinish()->Getname()<<" "<<roads[i]->Getdistance()<<endl;
    }
}

void WorldMap::showMap() {
	int sizeY = 800;
	int sizeX = 800;
	int boxSize = 80;
	int rows = sizeY;
	int columns = sizeX;
	cv::Vec3b black = cv::Vec3b(0,0,0);

	for(int i = 1; i < sizeX/boxSize; ++i) {
		for(int j = 0; j < this->worldMap.rows; ++j){
			this->worldMap(j,boxSize*i) = black;
		}
	}
	for(int i = 1; i < sizeY/boxSize; ++i) {
		for(int j = 0; j < this->worldMap.cols; ++j){
			this->worldMap(boxSize*i,j) = black;
		}
	}
	for(int i = 0; i < towns.size(); ++i) {
		cv::Point p = cv::Point(towns[i]->GetX(),towns[i]->GetY());
		cv::circle(this->worldMap, p, 5, cv::Scalar(255,0,0),-1);
		cv::putText(this->worldMap,towns[i]->Getname(),p,cv::FONT_HERSHEY_SIMPLEX,1,cv::Scalar(255,0,0),2);
	}
	for(int i = 0; i < towns.size()-1; ++i) {
		for(int j = i+1; j < towns.size(); ++j) {
			cv::Point p1 = cv::Point(towns[i]->GetX(),towns[i]->GetY());
			cv::Point p2 = cv::Point(towns[j]->GetX(),towns[j]->GetY());
			cv::line(this->worldMap, p1, p2, cv::Scalar(0,255,0), 2);
		}
	}


	//cv::imshow("mapa", this->worldMap);

}

void WorldMap::drawPath(vector<Town*> path, cv::Scalar color, int thickness) {
    cv::Point p1;
    cv::Point p2;
    for(int i = 0; i < path.size()-1; ++i) {
        p1 = cv::Point(path[i]->GetX(),path[i]->GetY());
        p2 = cv::Point(path[i+1]->GetX(),path[i+1]->GetY());
        cv::line(this->worldMap, p1, p2, color, thickness);
        cv::circle(this->worldMap, p1, 5, color,-1);
    }
    p1 = cv::Point(path[path.size()-1]->GetX(),path[path.size()-1]->GetY());
    cv::circle(this->worldMap, p1, 10, cv::Scalar(0,0,255),-1);
    p1 = cv::Point(path[0]->GetX(),path[0]->GetY());
    cv::circle(this->worldMap, p1, 10, cv::Scalar(0,0,0),-1);
}

vector<int> WorldMap::generateRandomPath(int display) {
    double x;
	struct drand48_data randBuffer;
	srand48_r(time(NULL), &randBuffer);

	vector<int> townsPossible;

	for(int i = 1; i < towns.size(); ++i) {
        townsPossible.push_back(towns[i]->Getid());
	}
	vector<int> path;
	path.push_back(towns[0]->Getid());

	for(int i = 0; i < towns.size()-1;++i) {
        drand48_r(&randBuffer, &x);
        x = (int)(x*1000.0)%townsPossible.size();
        path.push_back(townsPossible[x]);
        townsPossible.erase(townsPossible.begin()+x);
	}

	if(display == 1) {
        cout<<"Path: ";
        for(int i = 0; i < path.size(); ++i) {
            cout<<path[i]<<", ";
        }
        cout<<endl;
	}

	return path;

}

/*void WorldMap::generateRandomPaths(int iterations, int display) {
    double x;
	struct drand48_data randBuffer;
	srand48_r(time(NULL), &randBuffer);

	vector<int> townsPossible;
	vector<int> path;

    for(int iter = 0; iter < iterations; ++iter) {
        townsPossible.clear();
        path.clear();
        for(int i = 1; i < towns.size(); ++i) {
            townsPossible.push_back(towns[i]->Getid());
        }

        path.push_back(towns[0]->Getid());

        for(int i = 0; i < towns.size()-1;++i) {
            drand48_r(&randBuffer, &x);
            x = (int)(x*1000.0)%townsPossible.size();
            path.push_back(townsPossible[x]);
            townsPossible.erase(townsPossible.begin()+x);
        }

        this->paths.push_back(path);

        if(display == 1) {
            cout<<"Path: ";
            for(int i = 0; i < path.size(); ++i) {
                cout<<path[i]<<", ";
            }
            cout<<"dlugosc: "<<computePathLength(path)<<endl;
        }
    }

}*/

void WorldMap::generateRandomPaths(int iterations, int display) {
    double x;
	struct drand48_data randBuffer;
	srand48_r(time(NULL), &randBuffer);

	vector<Town*> townsPossible;
	vector<Town*> path;

    for(int iter = 0; iter < iterations; ++iter) {
        townsPossible.clear();
        path.clear();
        for(int i = 1; i < towns.size(); ++i) {
            townsPossible.push_back(towns[i]);
        }

        path.push_back(towns[0]);

        for(int i = 0; i < towns.size()-1;++i) {
            drand48_r(&randBuffer, &x);
            x = (int)(x*1000.0)%townsPossible.size();
            path.push_back(townsPossible[x]);
            townsPossible.erase(townsPossible.begin()+x);
        }

        this->paths.push_back(path);

        if(display == 1) {
            cout<<"Path: ";
            for(int i = 0; i < path.size(); ++i) {
                cout<<path[i]->Getid()<<", ";
            }
            cout<<"dlugosc: "<<computePathLength(path)<<endl;
        }
    }

}

/*int WorldMap::roadLength(int t1, int t2) {
    for(int i = 0; i < roads.size(); ++i) {
        int s = roads[i]->Getstart()->Getid();
        int f = roads[i]->Getfinish()->Getid();
        if((s == t1 && f == t2) || (s == t2 && f == t1)) {
            return roads[i]->Getdistance();
        }
    }
    return -1;
}*/

/*int WorldMap::computePathLength(vector<int> path) {
    int length = 0;
    for(int i = 0; i < path.size()-1;++i) {
        length += roadLength(path[i],path[i+1]);
    }
    return length;
}*/

int WorldMap::roadLength(Town* t1, Town* t2) {
    for(int i = 0; i < roads.size(); ++i) {
        int s = roads[i]->Getstart()->Getid();
        int f = roads[i]->Getfinish()->Getid();
        if((s == t1->Getid() && f == t2->Getid()) || (s == t2->Getid() && f == t1->Getid())) {
            return roads[i]->Getdistance();
        }
    }
    return -1;
}

int WorldMap::computePathLength(vector<Town*> path) {
    int length = 0;
    for(int i = 0; i < path.size()-1;++i) {
        length += roadLength(path[i],path[i+1]);
    }
    return length;
}

int WorldMap::findBestPath() {
    vector<Town*> best = this->paths[0];
    int bestLength = computePathLength(best);
    int ind = 0;
    for(int i = 1; i < this->paths.size(); ++i) {
        int length = computePathLength(this->paths[i]);
        if(length < bestLength) {
            bestLength = length;
            best = this->paths[i];
            ind = i;
        }
    }
    bestPath = paths[ind];
    return ind;
}

int WorldMap::findWorstPath() {
    vector<Town*> worst = this->paths[0];
    int worstLength = computePathLength(worst);
    int ind = 0;
    for(int i = 1; i < this->paths.size(); ++i) {
        int length = computePathLength(this->paths[i]);
        if(length > worstLength) {
            worstLength = length;
            worst = this->paths[i];
            ind = i;
        }
    }
    worstPath = paths[ind];
    return ind;
}



vector<Town*> WorldMap::adjustPath(vector<Town*> path) {
    vector<Town*> newPath;
    vector<Town*> possible;
    for(int i = 0; i < towns.size(); ++i) {
        possible.push_back(towns[i]);
    }

    for(int i = 0; i < path.size();++i) {
        int x = path[i]->GetX();
        int y = path[i]->GetY();
        int best = 0;
        int bestDist = (possible[0]->GetX() - x)*(possible[0]->GetX() - x) + (possible[0]->GetY() - y)*(possible[0]->GetY() - y);
        for(int i = 1; i < possible.size(); ++i) {
            int d = (possible[i]->GetX() - x)*(possible[i]->GetX() - x) + (possible[i]->GetY() - y)*(possible[i]->GetY() - y);
            if(d < bestDist) {
                bestDist = d;
                best = i;
            }
        }
        newPath.push_back(possible[best]);
        possible.erase(possible.begin()+best);
    }
    return newPath;
}

void WorldMap::changePath(int index, vector<Town*> path) {
    paths[index].clear();
    paths[index] = path;
}



















