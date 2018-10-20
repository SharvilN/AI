//
// Created by sharvil on 17/9/18.
//

#ifndef AIASSIGNMENT2_SIMULATEDANNEALING_H
#define AIASSIGNMENT2_SIMULATEDANNEALING_H


using namespace std;

class SimulatedAnnealing {

public:
    double **paperToSessionMatrix;
    double **distanceMatrix;
    int parallelSessions;
    int maxPapersInSession;
    int tradeOffCoefficient;
    int numberOfPapers;
    std::default_random_engine randomEngine;
    std::uniform_int_distribution<std::default_random_engine::result_type> uniformIntDistribution;

    SimulatedAnnealing(double** inputDistanceMatrix, int parallelSessions, int maxPapersInSession, int tradeOffCoefficient, int numberOfPapers);
    double goodness(int* state);
    int* randomInitialise();
    int* simulatedAnnealing(double duration);
    pair<int, int> nextState();
    int* updateState(int indexOne, int indexTwo, int* state);
    void initialiseDistanceMatrix(double **inputDistanceMatrix);
    void initialisePaperToSessionMatrix(int* state);
    double changeInScore(int indexOne, int indexTwo, int* state);
    vector<vector<int> > stateToSession(int *state);

};


#endif //AIASSIGNMENT2_SIMULATEDANNEALING_H
