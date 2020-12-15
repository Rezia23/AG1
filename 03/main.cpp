#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <set>
using namespace std;

using WeekNum = int;

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
public:
    int requiredPopularity;

    Election(int popularity) {
        requiredPopularity = popularity;
    }
    void pretty_print(){
        cout << "min popularity: " << requiredPopularity;
    }
};

enum ActionType{ZAVEST, ZRUSIT};

class Action{
public:
    int actionNum;
    ActionType type;
};


class Week{
public:
    WeekNum week;
    vector<Action> actions{};
    int numInfected;
    int popularity;
    Week (WeekNum week, int numInfected, int popularity){
        this->week = week;
        this->numInfected = numInfected;
        this->popularity = popularity;
    }
    //todo void addAction()
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
    map<WeekNum, Election> elections;
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
    
    shared_ptr<Week> finalWeek;
};


//this is where all the magic happens (eventually)
bool findSolutions(const Week & prevWeek, int currInfected, int currPopularity,
        int currNumMeasures, set<int> & canceledMeasures, SetUp & setup){
    WeekNum currWeekNum = prevWeek.week + 1;


    //should be at the end, if input is not already calculated for this week (as currPopularity)
    if(currNumMeasures > setup.maxNumActiveMeasures){
        return false;
    }
    if(currPopularity < 0){
        return false;
    }
    if(setup.elections.find(currWeekNum) != setup.elections.end()){
      if(currPopularity < setup.elections.find(currWeekNum)->second.requiredPopularity){
          return false;
      }
    }
    //todo
}



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
        Week firstWeek = Week()
        set<int> canceledMeasures = {};

        findSolutions(beginningAction,setup.numInfected, setup.startPopularity, 0, canceledMeasures, setup)

    }



    return 0;
}
