#include <iostream>
#include <vector>
#include <map>
using namespace std;

using Week = int;

class Measure{
public:
    int introducePopChange;
    int activePopChange;
    int cancelPopChange;
    double reproductionChange;
    void pretty_print(){
        cout << introducePopChange << " " << activePopChange << " " << cancelPopChange << " " << reproductionChange;
    }
};

class Election{
    int minPopularity;
public:
    Election(int popularity) {
        minPopularity = popularity;
    }
    void pretty_print(){
        cout << "min popularity: " << minPopularity;
    }
};



class SetUp{
public:
    int weeks;
    int maxNumActiveMeasures;
    int numMeasures;
    int numElections;
    int startPopularity;
    int numInfected;
    double reproduction;
    vector<Measure> measures;
    map<Week, Election> elections;
    void pretty_print(){
        cout << "Num weeks " <<weeks << "\nNum max active " << maxNumActiveMeasures << "\nnum measures "
        << numMeasures << "\nnumElections " << numElections << "\nstart popularity " << startPopularity
        << "\nnumInfected " << numInfected << "\nreproduction " << reproduction;
        for(auto a : measures){
            cout <<  "\n Measure: ";
            a.pretty_print();
        }
        for(auto a : elections){
            cout << "\nelection ";
            cout << a.first << " ";
            a.second.pretty_print();
        }
        cout << endl;
    }
};


int main() {
    int numInputs;
    cin >> numInputs;
    for(int inputNum = 0; inputNum < numInputs; inputNum++){
        //read input
        SetUp setup;
        cin >> setup.weeks >> setup.maxNumActiveMeasures >>
            setup.numMeasures >> setup.numElections >> setup.startPopularity >> setup.numInfected >> setup.reproduction;
        for(int i = 0; i<setup.numMeasures;i++){
            Measure measure;
            cin >> measure.introducePopChange >> measure.activePopChange >> measure.cancelPopChange >> measure.reproductionChange;
            setup.measures.push_back(measure);
        }
        for(int i = 0; i<setup.numElections; i++){
            int week, popNeeded;
            cin >> week >> popNeeded;
            setup.elections.insert(make_pair(week, Election(popNeeded)));
        }
        setup.pretty_print();

        //calculate all the stuff

    }



    return 0;
}
