#include <iostream>
#include<memory>
#include<vector>
class Vertex;

enum PlanetType{BEGINNING, END, INFECTED, NORMAL};

class Vertex{
    int planetID;
    PlanetType type;
    std::shared_ptr<Vertex> prev_vertex;
    Vertex (int planetID, PlanetType type){
        this->planetID = planetID;
        this->type = type;
    }


};



int main() {

    int numPlanets, numEdges;
    int beginning, end, numDaysSurvived;
    int numPlanetsInfected;
    int numHospitals;
    std::vector<std::vector<int>> edges;
    return 0;
}
