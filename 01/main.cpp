#include <iostream>
#include<memory>
#include<vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <set>
using namespace std;

class Vertex;

enum PlanetType {
    BEGINNING, END, INFECTED, NORMAL, HOSPITAL
};
enum State {
    UNVISITED, OPEN, CLOSED
};

class Visit{
public:
   int planetID;
   int distanceSomewhere = 0;
   shared_ptr<Visit> prev_visit;
   Visit(int planetID, int distanceSomewhere,  shared_ptr<Visit> prev_visit){
       this->planetID = planetID;
       this->distanceSomewhere = distanceSomewhere;
       this->prev_visit = prev_visit;
   }
};


class Vertex {
public:
    int planetID;
    PlanetType type;
    vector<shared_ptr<Visit>> visits;
    State state = UNVISITED;
    vector<int> neighbours;

    Vertex(int planetID){
        this->planetID = planetID;
        this->type = NORMAL;
    }

    Vertex(int planetID, PlanetType type, const vector<int> &neighbours) {
        this->planetID = planetID;
        this->type = type;
        this->neighbours = neighbours;
    }

    int numNeighbours() const {
        return this->neighbours.size();
    }


    void print() {
        cout << "id: " << planetID << " type: " << type
             << "neighbours: ";
        for (auto n: neighbours) {
            cout << n;
        }
        cout << endl;
    }
};

void findWayFromReachable(const shared_ptr<Visit> & visit, int end){

    shared_ptr<Visit> prev = visit;
    shared_ptr<Visit> next = visit->prev_visit;
    cout<<prev->planetID;
    while (next->planetID != end) {
        cout<<" "<<next->planetID;
        prev = next;
        next = next->prev_visit;
    }
    cout << " " << next->planetID;
}

bool bfsFromEnd(const vector<shared_ptr<Vertex>> &graph, int end, int range, int beginning){
    queue<shared_ptr<Visit>> q;

    q.push(make_shared<Visit>(Visit(end, 0, nullptr)));
    graph[end]->state = OPEN;
    while (!q.empty()) {
        shared_ptr<Visit> currentVisit = q.front();
        q.pop();
        int currID = currentVisit->planetID;
        if (graph[currID]->type == BEGINNING) {
            //todo
            findWayFromReachable(currentVisit, end);
            return true;
        }
        if(graph[currID]->type == HOSPITAL){
            currentVisit->distanceSomewhere = 0;
        }
        for (int i = 0; i < graph[currID]->numNeighbours(); i++) {
            int neighbour = graph[currID]->neighbours[i];
            if(graph[neighbour]->type == INFECTED && currentVisit->distanceSomewhere+1 >range){
                continue;
            }
            if (graph[neighbour]->state == UNVISITED || (graph[neighbour]->type!= END && graph[neighbour]->visits.back()->distanceSomewhere > currentVisit->distanceSomewhere + 1)) {
                shared_ptr<Visit> nextVisit = make_shared<Visit>(Visit(neighbour,currentVisit->distanceSomewhere +1, currentVisit ));
                q.push(nextVisit);
                graph[neighbour]->visits.push_back(nextVisit);
                graph[neighbour]->state = OPEN;
            }
        }
        graph[currID]->state = CLOSED;
    }
    return false;

}

//void printResults(const vector<shared_ptr<Vertex>> & graph, int beginning, int end){
//    int next = end;
//    vector<int> results;
//    while (next != beginning) {
//        results.push_back(graph[next]->planetID);
//        next = graph[next]->prev_vertex;
//    }
//    results.push_back(graph[next]->planetID);
//    for (size_t i = results.size() - 1; i > 0; i--) {
//        cout << results[i] << " ";
//    }
//    cout << results[0];
//}

void readTypes(int numPlanets, const vector<shared_ptr<Vertex>> &graph, PlanetType type) {
    for(int i = 0; i<numPlanets;i++){
        int tmp;
        cin>>tmp;
        graph[tmp]->type = type;
    }
}


void readEdges(int numEdges, const vector<shared_ptr<Vertex>> & graph) {
    for(int i = 0; i<numEdges;i++){
        int tmp1, tmp2;
        cin >> tmp1 >> tmp2;
        graph[tmp1]->neighbours.push_back(tmp2);
        graph[tmp2]->neighbours.push_back(tmp1);
    }
}

int main() {
    int numPlanets, numEdges;
    int beginning, end, numDaysSurvived;
    int numPlanetsInfected;
    int numHospitals;

    cin >> numPlanets >> numEdges >> beginning >> end >> numDaysSurvived >> numPlanetsInfected;
    vector<shared_ptr<Vertex>> graph;
    graph.reserve(numPlanets);

    for (int i = 0; i < numPlanets; i++) {
        graph.push_back(make_shared<Vertex>(i));
    }
    graph[beginning]->type = BEGINNING;
    graph[end]->type = END;

    if (numPlanetsInfected > 0) {
        readTypes(numPlanetsInfected, graph, INFECTED);

    }
    cin >> numHospitals;
    if (numHospitals > 0) {
        readTypes(numPlanetsInfected, graph, HOSPITAL);
    }
    readEdges(numEdges, graph);


    ////////////////////////////////bfs around end
    if (bfsFromEnd(graph, end, numDaysSurvived, beginning)) {
        return 0;
    }
    cout<<-1;

    return 0;
}



