#include <bits/stdc++.h>
#include "SimulatedAnnealing.h"

using namespace std;

int main(){
//    ifstream fin("in");
    int n, c, k, p;
    cout << "Enter Input : " << endl;
    cin >> n >> c >> k >> p;
    double** distanceMatrix;
    distanceMatrix = new double* [n];
    for(int i=0; i<n; i++){
        distanceMatrix[i] = new double[n];
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cin >> distanceMatrix[i][j];
        }
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << distanceMatrix[i][j] << " ";
        }
        cout << endl;
    }
    SimulatedAnnealing simulatedAnnealingAgent(distanceMatrix, p, k, c, n);
    int* state = simulatedAnnealingAgent.simulatedAnnealing(5);


    cout << endl;
}