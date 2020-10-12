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
    int prev_vertex{};
    int following_vertex = -1;
    bool reachableEndWhenInfected = false;
    int distanceFromEnd{};
    int distanceFromStart{};
    State dfsState = UNVISITED;
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

    void setDFSState(State state) {
        this->dfsState = state;
    }

    void pushToQueue(int following, int distance) {
        this->following_vertex = following;
        this->distanceFromEnd = distance;
    }

    void pushToStack(int previous, int distance) {
        this->prev_vertex = previous;
        this->distanceFromStart = distance;
    }

    void print() {
        cout << "id: " << planetID << " type: " << type << " stateDFS, BFS: " << this->dfsState
             << "neighbours: ";
        for (auto n: neighbours) {
            cout << n;
        }
        cout << endl;
    }
};

void findWayFromReachable(const vector<shared_ptr<Vertex>> & graph, int neighbour){
    int prev = neighbour;
    int next = graph[neighbour]->following_vertex;
    while (graph[next]->type != END) {
        graph[next]->prev_vertex = prev;
        prev = next;
        next = graph[next]->following_vertex;
    }
    graph[next]->prev_vertex = prev;
}

bool bfsInRange(const vector<shared_ptr<Vertex>> &graph, int end, int range, int beginning) {
    queue<int> q;
    q.push(end);
    graph[end]->distanceFromEnd = 0;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        graph[curr]->reachableEndWhenInfected = true;
        if (graph[curr]->type == BEGINNING) {
            findWayFromReachable(graph, curr);
            return true;
        }
        if (graph[curr]->distanceFromEnd >= range) {
            return false;
        }
        for (int i = 0; i < graph[curr]->numNeighbours(); i++) {
            int neighbour = graph[curr]->neighbours[i];

            if (!graph[neighbour]->reachableEndWhenInfected) {
                q.push(neighbour);
                graph[neighbour]->pushToQueue(curr, graph[curr]->distanceFromEnd + 1);
            }
        }
    }
    return false;
}


bool dfsInRange(const vector<shared_ptr<Vertex>> &graph, int beginning, int end, int numDaysSurvived) {
    stack<int> s;
    s.push(beginning);
    graph[beginning]->distanceFromStart = 0;
    while (!s.empty()) {
        int curr = s.top();
        s.pop();

        if (graph[curr]->type == END) {
            return true;
        }
        if (graph[curr]->reachableEndWhenInfected &&
            graph[curr]->distanceFromEnd + graph[curr]->distanceFromStart + 1 <= int(graph.size())) { //todo for bonus should be max(3dn, n)
            findWayFromReachable(graph, curr);
            return true;
        }
        //TODO: for bonus should be shorter than max(3d*n,n)
        if (graph[curr]->distanceFromStart + numDaysSurvived >= int(graph.size())) {
            continue;
        }
        graph[curr]->setDFSState(OPEN);
        for (int i = 0; i < graph[curr]->numNeighbours(); i++) {
            int neighbour = graph[curr]->neighbours[i];
            if (graph[neighbour]->dfsState == UNVISITED) {
                if (graph[neighbour]->type == INFECTED && !graph[neighbour]->reachableEndWhenInfected) {
                    continue;
                }
                s.push(neighbour);
                graph[neighbour]->pushToStack(curr, graph[curr]->distanceFromStart + 1);

            }
        }
        graph[curr]->setDFSState(CLOSED);
    }
    return false;
}

void printResults(const vector<shared_ptr<Vertex>> & graph, int beginning, int end){
    int next = end;
    vector<int> results;
    while (next != beginning) {
        results.push_back(graph[next]->planetID);
        next = graph[next]->prev_vertex;
    }
    results.push_back(graph[next]->planetID);
    for (size_t i = results.size() - 1; i > 0; i--) {
        cout << results[i] << " ";
    }
    cout << results[0];
}

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
    if (bfsInRange(graph, end, numDaysSurvived, beginning)) {
        printResults(graph, beginning, end);
        return 0;
    }

    for(int i = 0; i<numHospitals;i++){
//        bfsAroundHospital();
    }



    ///////////////////////////////dfs from beginning
    if (!dfsInRange(graph, beginning, end, numDaysSurvived)) {
        cout << -1;
    } else {
        printResults(graph, beginning, end);
    }
    return 0;
}



