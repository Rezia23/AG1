#include <iostream>
#include<memory>
#include<vector>
#include <algorithm>
#include <queue>
#include <stack>
using namespace std;
class Vertex;

enum PlanetType{BEGINNING, END, INFECTED, NORMAL, HOSPITAL};
enum State{UNVISITED, OPEN, CLOSED};
class Vertex{
public:
    int planetID;
    PlanetType type;
    int prev_vertex;
    int following_vertex = -1;
    bool reachableEndWhenInfected = false;
    int distanceFromEnd;
    int distanceFromStart;
    State bfsState = UNVISITED;
    State dfsState = UNVISITED;
    vector<int> neighbours;


    Vertex (int planetID, PlanetType type, vector<int> neighbours){
        this->planetID = planetID;
        this->type = type;
        this->neighbours = neighbours;
    }

    int numNeighbours() const{
        return this->neighbours.size();
    }
    void setBFSState(State state){
        this->bfsState = state;
    }
    void setDFSState(State state){
        this->dfsState = state;
    }
    void pushToQueue(int following, int distance){
        this->following_vertex = following;
        this->distanceFromEnd = distance;
        this->reachableEndWhenInfected = true;
    }
    void pushToStack(int previous, int distance){
        this->prev_vertex = previous;
        this->distanceFromStart = distance;
    }
    void print(){
        cout<< "id: "<<planetID << " type: " << type <<" stateDFS, BFS: " << this->dfsState << this->bfsState << "neighbours: ";
        for(auto n: neighbours){
            cout<<n;
        }
        cout<<endl;
    }
};


std::vector<int> readValues(int numValues) {
    std::vector<int> values;
    for (int i = 0; i<numValues;i++){
        int tmp;
        std::cin>>tmp;
        values.push_back(tmp);
    }
    return values;
}

std::vector<std::vector<int>> readEdges(int numEdges) {
    std::vector<std::vector<int>> edges;
    edges.resize(numEdges);
    for (int i = 0; i<numEdges;i++){
        int tmp1, tmp2;
        std::cin>>tmp1>>tmp2;
        edges[i].push_back(tmp1);
        edges[i].push_back(tmp2);
    }
    return edges;
}
PlanetType findPlanetType(int planet, int beginning, int end, vector<int> infected, vector<int> hospitals){
    if(planet==beginning){
        return BEGINNING;
    }
    if(planet==end){
        return END;
    }
    auto it = find (infected.begin(), infected.end(), planet);
    if (it != infected.end())
        return INFECTED;
    it = find (hospitals.begin(), hospitals.end(), planet);
    if (it != hospitals.end())
        return HOSPITAL;
    return NORMAL;
}


vector<int> findPlanetNeighbours(int planet, vector<vector<int>> edges) {
    vector<int> neighbours;
    for (auto edge : edges){
        if(edge[0]==planet){
            neighbours.push_back(edge[1]);
        }else if(edge[1]==planet){
            neighbours.push_back((edge[0]));
        }
    }
    return neighbours;
}

void bfsInRange(vector<shared_ptr<Vertex>> & graph, int end, int range, int beginning) {
    queue<int> q;
    q.push(end);
    graph[end]->distanceFromEnd = 0;
    while(!q.empty()){
        int curr = q.front();
        q.pop();
        if(graph[curr]->distanceFromEnd>=range){
            break;
        }
        //TODO: possible check for beginning
        graph[curr]->setBFSState(OPEN);
        for(int i = 0; i< graph[curr]->numNeighbours(); i++){
            int neighbour = graph[curr]->neighbours[i];
            if(neighbour==beginning){
                //TODO
            }
            if(graph[neighbour]->bfsState == UNVISITED){
                q.push(neighbour);
                graph[neighbour]->pushToQueue(curr, graph[curr]->distanceFromEnd+1);
            }
        }
        graph[curr]->setBFSState(CLOSED);
    }
}

bool dfsInRange(vector<shared_ptr<Vertex>> & graph, int beginning, int end, int numDaysSurvived) {
    stack<int> s;
    s.push(beginning);
    graph[beginning]->distanceFromStart = 0;
    while(!s.empty()){
        int curr = s.top();
        s.pop();
        //TODO: for bonus should be shorter than max(3d*n,n)
        if(graph[curr]->distanceFromStart==int(graph.size())){
            continue;
        }
        graph[curr]->setDFSState(OPEN);
        for(int i = 0; i< graph[curr]->numNeighbours(); i++){
            int neighbour = graph[curr]->neighbours[i];
            if(graph[neighbour]->dfsState == UNVISITED){
                if(graph[neighbour]->type == END){
                    graph[neighbour]->pushToStack(curr, graph[curr]->distanceFromEnd+1);
                    return true;
                }
                ///////////begining of ugly copy paste
                if(graph[neighbour]->reachableEndWhenInfected){
                    graph[neighbour]->pushToStack(curr, graph[curr]->distanceFromEnd+1);
                    int prev = neighbour;
                    int next = graph[neighbour]->following_vertex;
                    while(graph[next]->type != END){
                        graph[next]->prev_vertex = prev;
                        prev = next;
                        next = graph[next]->following_vertex;
                    }
                    graph[next]->prev_vertex = prev;

                    return true;
                }
                /////////////end of ugly copy paste
                if(graph[neighbour]->type==INFECTED){
                    if(!graph[neighbour]->reachableEndWhenInfected || graph[neighbour]->distanceFromEnd + graph[curr]->distanceFromStart + 1 > int(graph.size())){ //todo should also be max(3dn, n)
                        continue;
                    }else{
                        graph[neighbour]->pushToStack(curr, graph[curr]->distanceFromEnd+1);
                        int prev = neighbour;
                        int next = graph[neighbour]->following_vertex;
                        while(graph[next]->type != END){
                            graph[next]->prev_vertex = prev;
                            prev = next;
                            next = graph[next]->following_vertex;
                        }
                        graph[next]->prev_vertex = prev;

                        return true;
                    }
                }
                s.push(neighbour);
                graph[neighbour]->pushToStack(curr, graph[curr]->distanceFromEnd+1);

            }
        }
        graph[curr]->setDFSState(CLOSED);
    }
    return false;
}



int main() {

    int numPlanets, numEdges;
    int beginning, end, numDaysSurvived;
    int numPlanetsInfected;
    int numHospitals;
    std::vector<std::vector<int>> edges;
    std::vector<int> infectedPlanets;
    std::vector<int> hospitals;

    //TODO: decompose
    std::cin >> numPlanets >> numEdges;
    std::cin >> beginning >> end >> numDaysSurvived;
    std::cin >> numPlanetsInfected;
    if (numPlanetsInfected>0){
        infectedPlanets = readValues(numPlanetsInfected);
    }
    std::cin >> numHospitals;
    if (numHospitals>0){
        hospitals = readValues(numHospitals);
    }
    edges = readEdges(numEdges);

    std::vector<std::shared_ptr<Vertex>> graph;
    graph.resize(numPlanets);
    for (int i = 0; i<numPlanets;i++){
        graph[i] =
    make_shared<Vertex>(i, findPlanetType(i, beginning, end, infectedPlanets, hospitals), findPlanetNeighbours(i, edges));
    }
    bfsInRange(graph, end,numDaysSurvived, beginning);
    if(!dfsInRange(graph, beginning, end, numDaysSurvived)){
        cout<<-1;
    }else{
        int next = end;
        vector<int> results;
        while(next!=beginning){
            results.push_back(graph[next]->planetID);
            next = graph[next]->prev_vertex;
        }
        results.push_back(graph[next]->planetID);
        for(size_t i = results.size()-1; i>0;i--){
            cout<<results[i]<<" ";
        }
        cout<<results[0];
    }
    return 0;
}



