#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <cmath>

using namespace std;

const long long int MAX_INFECTED = 1073741824;
const long long int MIN_POPULARITY = 0;
const long long int MAX_POPULARITY = 100;
const double FLOOR_CONST = 0.00000001;

typedef tuple<long long int, vector<bool>, vector<bool>> t_MapKey;

struct t_SafetyMeasure {
public:
    long long int m_PopStart;
    long long int m_PopActive;
    long long int m_PopEnd;
    double m_dR;
public:
    t_SafetyMeasure()= default;
    t_SafetyMeasure(long long int s, long long int a, long long int e, double dr)
    :m_PopStart(s), m_PopActive(a), m_PopEnd(e), m_dR(dr){}
};

struct t_MapValue {
public:
    const t_MapKey *  m_MapKeyPtr;
    long long int m_Infected;
    bool m_Changed;
    int m_MeasureNumber;
    bool m_ChangedToTrue;
public:
    t_MapValue() = default;
    t_MapValue(const t_MapKey * tPtr, long long int i, bool c, long long int mn, bool ctt)
    :m_MapKeyPtr(tPtr), m_Infected(i), m_Changed(c), m_MeasureNumber(mn), m_ChangedToTrue(ctt){}
};

bool checkValues(const long long int& week,
                 const long long int& infectedIfUsed,
                 const long long int& popularityIfUsed,
                 const map<long long int, long long int>& v_Elections,
                 const vector<bool>& activeMeasures,
                 const long long int& maxNumOfActiveMeasures);

long long int calculatePopularity(const long long int& popularityLastWeek,
                                  const vector<bool>& activeMeasures,
                                  const vector<t_SafetyMeasure>& v_SafetyMeasures,
                                  size_t flipped);

long long int calculateInfected(const long long int& infLastWeek,
                                const double& globalR,
                                const vector<bool>& activeMeasures,
                                const vector<t_SafetyMeasure>& v_SafetyMeasures);

void saveTheGalaxy(const long long int& N,
                   const long long int& A,
                   const long long int& O,
                   const long long int& F,
                   const long long int& V,
                   const double& R,
                   const vector<t_SafetyMeasure>& v_SafetyMeasures,
                   const map<long long int, long long int>& v_Elections);

void readInput(long long int& N,
               long long int& A,
               long long int& O,
               long long int& P,
               long long int& F,
               long long int& V,
               double& R,
               vector<t_SafetyMeasure>& v_SafetyMeasures,
               map<long long int, long long int>& v_Elections);

int main() {
    long long int T; // number of inputs to handle
    long long int N; // number of weeks
    long long int A; // max num of active safety measures
    long long int O; // num of safety measures
    long long int P; // num of elections
    long long int F; // starting popularity
    long long int V; // num of infected at end of 1st week
    double R; // global virus spread speed
    vector<t_SafetyMeasure> v_SafetyMeasures;
    map<long long int, long long int> v_Elections;

    cin >> T;

    for (long long int i = 0; i < T; ++i) {
        v_SafetyMeasures.clear();
        v_Elections.clear();
        readInput(N, A, O, P, F, V, R, v_SafetyMeasures, v_Elections);
        //cout<<N<<" "<<A<<" "<<O<<" "<<P<<" "<<F<<" "<<V<<" "<<R<<endl<<v_SafetyMeasures.size()<<endl<<v_Elections.size()<<endl;
        saveTheGalaxy(N, A, O, F, V, R, v_SafetyMeasures, v_Elections);
    }

    return 0;
}

void readInput(long long int& N, long long int& A, long long int& O, long long int& P, long long int& F, long long int& V, double& R,
               vector<t_SafetyMeasure>& v_SafetyMeasures, map<long long int, long long int>& v_Elections ){
    cin >> N >> A >> O >> P >> F >> V >> R;
    for (long long int i = 0; i < O; ++i) {
        long long int s, a, e;
        double dr;
        cin >> s >> a >> e >> dr;
        t_SafetyMeasure op(s, a, e, dr);
        v_SafetyMeasures.push_back(op);
    }
    for (long long int j = 0; j < P; ++j) {
        long long int t, p;
        cin >> t >> p;
        v_Elections.insert(make_pair(t, p));
    }
}

void saveTheGalaxy(const long long int& N, const long long int& A, const long long int& O, const long long int& F, const long long int& V, const double& R,
                   const vector<t_SafetyMeasure>& v_SafetyMeasures, const map<long long int, long long int>& v_Elections){
    // Memory
    vector<map<t_MapKey ,t_MapValue>> memory(N);

    vector<bool> activeMeasuresFirstWeek(O, false);
    vector<bool> usedMeasuresFirstWeek(O, false);
    t_MapValue firstWeekVal(nullptr, V, false, 7, false);
    // week , popularity, active rules, used rules
    t_MapKey mapKeyTuple(F, activeMeasuresFirstWeek, usedMeasuresFirstWeek);

    memory[0].insert(make_pair(mapKeyTuple, firstWeekVal));

    // careful weeks indexed from 1 at output
    for (long long int i = 1; i < N; ++i) {
        for (auto & currentState : memory[i - 1]) {
            // try for no changes
            {
                // variables
                long long int currentPopularity     = get<0>(currentState.first);
                const vector<bool>& currentActive   = get<1>(currentState.first);
                const vector<bool>& currentUsed     = get<2>(currentState.first);
                long long int currentInfected       = currentState.second.m_Infected;
                // calculate next week with no changes
                long long int infectedNextWeek = calculateInfected(currentInfected, R, currentActive, v_SafetyMeasures);
                long long int popularityNextWeek = calculatePopularity(currentPopularity, currentActive, v_SafetyMeasures, 7); // 7 out of range
                bool passCheck = checkValues(i+1, infectedNextWeek, popularityNextWeek, v_Elections, currentActive, A);
                // if passed check for duplicates and write long long into memory
                if (passCheck){
                    // vals to save for next week
                    const t_MapKey * tuplePtr = &currentState.first;
                    t_MapValue saveValue(tuplePtr, infectedNextWeek, false, 7, false);
                    t_MapKey newMapKeyTuple(popularityNextWeek, currentActive, currentUsed);
                    // check for duplicates
                    if (memory[i].find(newMapKeyTuple) != memory[i].end()){
                        if (saveValue.m_Infected < memory[i][newMapKeyTuple].m_Infected)
                            memory[i][newMapKeyTuple] = saveValue;
                    } else {
                        memory[i].insert(make_pair(newMapKeyTuple, saveValue));
                    }
                }
            }
            // iterate trough active measures, always flipping a single measure on or off
            for (long long int j = 0; j < O; ++j) {
                // variables
                long long int currentPopularity     = get<0>(currentState.first);
                const vector<bool>& currentUsed     = get<2>(currentState.first);
                long long int currentInfected       = currentState.second.m_Infected;
                vector<bool> nextWeekActive         = get<1>(currentState.first);
                vector<bool> nextWeekUsed           = get<2>(currentState.first);
                // flip a bit
                if (!nextWeekActive[j] && !currentUsed[j]){
                    nextWeekActive[j] = true;
                } else if (nextWeekActive[j]){
                    nextWeekActive[j] = false;
                    nextWeekUsed[j] = true;
                } if (currentUsed[j]){
                    continue;
                }
                // calculate next week
                long long int infectedNextWeek = calculateInfected(currentInfected, R, nextWeekActive, v_SafetyMeasures);
                long long int popularityNextWeek = calculatePopularity(currentPopularity, nextWeekActive, v_SafetyMeasures, j);
                bool passCheck = checkValues(i+1, infectedNextWeek, popularityNextWeek, v_Elections, nextWeekActive, A);
                if (passCheck){
                    const t_MapKey * tuplePtr = &currentState.first;
                    t_MapValue saveValue(tuplePtr, infectedNextWeek, true, j, nextWeekActive[j]);
                    t_MapKey newMapKeyTuple(popularityNextWeek, nextWeekActive, nextWeekUsed);
                    if (memory[i].find(newMapKeyTuple) != memory[i].end()){
                        if (saveValue.m_Infected < memory[i][newMapKeyTuple].m_Infected)
                            memory[i][newMapKeyTuple] = saveValue;
                    } else {
                        memory[i].insert(make_pair(newMapKeyTuple, saveValue));
                    }
                }
            }
        }
    }

    if (memory[N-1].empty()){
        cout << "-1" << endl;
        return;
    }

    t_MapValue minVal = memory[N-1].begin()->second;
    for (auto & minState : memory[N-1]){
        if(minState.second.m_Infected < minVal.m_Infected)
            minVal = minState.second;
    }

    int numOfChanges = 0;
    long long int i = N-2;
    const t_MapKey * nextPtr = minVal.m_MapKeyPtr;
    vector<t_MapValue> path;

    if (minVal.m_Changed) {
        path.push_back(minVal);
        numOfChanges++;
    }
    while (nextPtr != nullptr){
        if(memory[i][*nextPtr].m_Changed)
            numOfChanges++;
        path.push_back(memory[i][*nextPtr]);
        nextPtr = memory[i][*nextPtr].m_MapKeyPtr;
        i--;
    }

    cout << numOfChanges << " " << minVal.m_Infected << endl;
    int k = 1;
    for (int j = path.size()-1; j >= 0; --j) {
        if (path[j].m_Changed) {
            if (path[j].m_ChangedToTrue) {
                cout << k << " " << "zavest" << " " << path[j].m_MeasureNumber << endl;
            } else {
                cout << k << " " << "zrusit" << " " << path[j].m_MeasureNumber << endl;
            }
        }
        k++;
    }

}

bool checkValues(const long long int& week, const long long int& infectedIfUsed, const long long int& popularityIfUsed,
                 const map<long long int, long long int>& v_Elections, const vector<bool>& activeMeasures, const long long int& maxNumOfActiveMeasures){
    // too many infected
    if (infectedIfUsed > MAX_INFECTED)
        return false;
    // popularity too low
    if (popularityIfUsed < MIN_POPULARITY)
        return false;

    // check popularity if its election week
    if(popularityIfUsed < (v_Elections.count(week) ? v_Elections.at(week) : 0))
        return false;
    // check if too many active
    long long int active = 0;
    for (bool activeMeasure : activeMeasures)
        if (activeMeasure)
            active++;
    // active check
    if (active > maxNumOfActiveMeasures)
        return false;
    // congratulations you made it
    return true;
}

long long int calculateInfected(const long long int& infLastWeek, const double& globalR, const vector<bool>& activeMeasures, const vector<t_SafetyMeasure>& v_SafetyMeasures){
    double finalR = globalR;
    for (size_t i = 0; i < v_SafetyMeasures.size(); ++i) {
        if(activeMeasures[i])
            finalR *= v_SafetyMeasures[i].m_dR;
    }
    return floor((FLOOR_CONST + (infLastWeek * finalR)));
}

long long int calculatePopularity(const long long int& popularityLastWeek, const vector<bool>& activeMeasures, const vector<t_SafetyMeasure>& v_SafetyMeasures, size_t flipped){
    long long int finalPopularity = popularityLastWeek;
    for (size_t i = 0; i < v_SafetyMeasures.size(); ++i) {
        if (flipped == i){
            if (activeMeasures[i]){
                finalPopularity += v_SafetyMeasures[i].m_PopStart;
            } else {
                finalPopularity += v_SafetyMeasures[i].m_PopEnd;
            }
        } else if (activeMeasures[i]){
            finalPopularity += v_SafetyMeasures[i].m_PopActive;
        }
    }
    if (finalPopularity > MAX_POPULARITY)
        return MAX_POPULARITY;
    return finalPopularity;
}
