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

class Vertex {
public:
    int planetID;
    PlanetType type;
    vector<int> following_vertex;
    int distanceSomewhere = 0;
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

void findWayFromReachable(const vector<shared_ptr<Vertex>> & graph, int beginning){
    cout<<beginning;
    int prev = beginning;
    int prePrev;
    int next = graph[beginning]->following_vertex.back();
    graph[beginning]->following_vertex.pop_back();
    while (graph[next]->type != END) {
        cout<<" " << next;
        prePrev = prev;
        prev = next;
        next = graph[prev]->following_vertex.back();
        graph[prev]->following_vertex.pop_back();
        while(prePrev == next && graph[prev]->type != HOSPITAL){
            next = graph[prev]->following_vertex.back();
            graph[prev]->following_vertex.pop_back();
        }
    }
    cout << " " << next;
}

bool bfsFromEnd(const vector<shared_ptr<Vertex>> &graph, int end, int range, int beginning){
    queue<int> q;
    q.push(end);
    graph[end]->distanceSomewhere = 0;
    graph[end]->state = OPEN;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        if (graph[curr]->type == BEGINNING) {
            findWayFromReachable(graph, curr);
            return true;
        }
        if(graph[curr]->type == HOSPITAL){
            graph[curr]->distanceSomewhere =0;
        }
        for (int i = 0; i < graph[curr]->numNeighbours(); i++) {
            int neighbour = graph[curr]->neighbours[i];
            if(graph[neighbour]->type == INFECTED && graph[curr]->distanceSomewhere+1 >range){
                continue;
            }
            if (graph[neighbour]->state == UNVISITED || graph[neighbour]->distanceSomewhere > graph[curr]->distanceSomewhere + 1) {
                q.push(neighbour);
                graph[neighbour]->following_vertex.push_back(curr);
                graph[neighbour]->distanceSomewhere = graph[curr]->distanceSomewhere + 1;
                graph[neighbour]->state = OPEN;
            }
        }
        graph[curr]->state = CLOSED;
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



