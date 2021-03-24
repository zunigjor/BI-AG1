#include <iostream>
#include <vector>
#include <queue>
#include <list>

using namespace std;

class CPlanet {
public:
    bool infected;
    bool wMedicine;
    vector<int> neighbors;
public:
    CPlanet():infected(false), wMedicine(false){}
    ~CPlanet() = default;
    void addNeighbor(int neighborID){neighbors.push_back(neighborID);}
};

class CUniverse {
private:
    int numOfPlanets;               // Vrcholy
    int startingPlanet;             // Start
    int numOfStopsAfterInfection;   // Pocet zastaveni ktere posadka prezije po navstiveni nakazene planety.
    vector<CPlanet> planets;        // Vektor s planetami
    vector<int> lives;              // Vektor s zivoty
    vector<bool> visited;           // Vektor s visited
public:
    CUniverse(int N, int s, int l):
            numOfPlanets(N),
            startingPlanet(s),numOfStopsAfterInfection(l),
            planets(numOfPlanets),
            lives(numOfPlanets, INT32_MIN),
            visited(numOfPlanets, false){}
    ~CUniverse() = default;
    void makeInfected(int x){
        planets.at(x).infected = true;
    }
    void makeMedic(int x){
        planets.at(x).wMedicine = true;
    }
    void addWay(int x, int y){
        planets.at(x).addNeighbor(y);
        planets.at(y).addNeighbor(x);
    }
    list<int> findPath(int goal);
};

list<int> CUniverse::findPath(int goal) {
    queue<int>  planetsQueue;
    vector<int> path (numOfPlanets, INT32_MIN);
    list<int> result;
    int pathStart = 0;
    // start
    planetsQueue.emplace(goal);
    lives.at(goal) = numOfStopsAfterInfection;
    while (path.at(startingPlanet) == INT32_MIN && !planetsQueue.empty() && !visited.at(startingPlanet)){
        int current = planetsQueue.front();
        visited.at(current) = true;
        for (auto & neighborPlanet : planets.at(current).neighbors){
            if ( !visited.at(neighborPlanet) || lives.at(neighborPlanet) < lives.at(current) - 1 ){ // not visited or visited with less hp
                if ( planets.at(neighborPlanet).infected && lives.at(current) - 1 >= 0 ){ // infected planet
                    lives.at(neighborPlanet) = lives.at(current) - 1;
                    path.at(neighborPlanet) = current;
                    planetsQueue.emplace(neighborPlanet);
                } else if ( !planets.at(neighborPlanet).infected
                         && !planets.at(neighborPlanet).wMedicine
                         && lives.at(neighborPlanet) < lives.at(current) - 1){ // normal
                    lives.at(neighborPlanet) = lives.at(current) - 1;
                    path.at(neighborPlanet) = current;
                    planetsQueue.emplace(neighborPlanet);
                } else if ( planets.at(neighborPlanet).wMedicine && lives.at(neighborPlanet) < lives.at(current) - 1){ // with medicine
                    pathStart = current;
                    list<int> tmp = findPath(neighborPlanet);
                    result.splice(result.end(), tmp);
                }
            }
        }
        planetsQueue.pop();
    }
    if (path.at(startingPlanet) != INT32_MIN){
        pathStart = startingPlanet;
        visited.at(startingPlanet) = true;
    }
    // kill recursions
    if (!visited.at(startingPlanet)){
        result.clear();
        return result;
    }
    // construct result from path
    int jmp = pathStart;
    while (jmp != INT32_MIN){
        result.push_back(jmp);
        jmp = path.at(jmp);
    }
    if (result.size() == 1 && result.front() == 0){
        result.clear();
    }
    return result;
}

int main() {
    int N, M, s, t, l, Inf, Med;
    cin >> N >> M;
    cin >> s >> t >> l;
    CUniverse universe (N, s, l);
    // read which planets are infected
    cin >> Inf;
    for (int i = 0; i < Inf; ++i) {
        int infectedID;
        cin >> infectedID;
        //if (infectedID == s || infectedID == t){
        //    cout << -1 << " Infected at:" << infectedID << " s:" << s << " t:" << t<< endl;
        //    return 0;
        //}
        universe.makeInfected(infectedID);
    }
    // read which planets have medicine
    cin >> Med;
    for (int i = 0; i < Med; ++i) {
        int medicID;
        cin >> medicID;
        //if (medicID == s || medicID == t){
        //    cout << -1 << " Medic at:" << medicID << " s:" << s << " t:" << t << endl;
        //    return 0;
        //}
        universe.makeMedic(medicID);
    }
    // read spaceways
    for (int i = 0; i < M; ++i) {
        int x, y;
        cin >> x >> y;
        universe.addWay(x, y);
    }
    // Check if start is goal
    if (s == t){
        cout << s << endl;
        return 0;
    }
    // Find path and print it
    list<int> result = universe.findPath(t);
    if (result.empty()){
        cout << -1 << endl;
        return 0;
    }
    for (auto & i : result) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}