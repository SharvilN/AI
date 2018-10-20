//
// Created by sharvil on 17/9/18.
//

#include <bits/stdc++.h>
#include "SimulatedAnnealing.h"

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<double> double_seconds;

void SimulatedAnnealing::initialiseDistanceMatrix(double** inputDistanceMatrix){
    distanceMatrix = new double*[numberOfPapers];
    for(int i=0; i<numberOfPapers; i++){
        distanceMatrix[i] = new double[numberOfPapers];
    }
    for(int i=0; i<numberOfPapers; i++){
        for(int j=0; j<numberOfPapers; j++){
            cout << inputDistanceMatrix[i][j] << " ";
            distanceMatrix[i][j] = inputDistanceMatrix[i][j];
            cout << distanceMatrix[i][j] << " ";

        }
    }
}

std::vector<std::vector<int> > SimulatedAnnealing::stateToSession(int* state)
{
    std::vector<std::vector<int> > blocks;
    int size = sizeof(state)/sizeof(state[0]);
    for (auto it = 0; it < parallelSessions*maxPapersInSession; it += maxPapersInSession)
        blocks.push_back(std::vector<int>(state + it, state + it + maxPapersInSession));

    return blocks;
}

void SimulatedAnnealing::initialisePaperToSessionMatrix(int* state){
    paperToSessionMatrix = new double*[numberOfPapers];
    for(int i=0; i<numberOfPapers; i++){
        paperToSessionMatrix[i] = new double[parallelSessions];
    }

    auto sessions = stateToSession(state);
//    cout << "Sessions : " << endl;
//    cout << "sessions [0] size : " << sessions[0].size() << endl;
//    for(int i=0; i<sessions.size(); i++){
//        for(int j=0; j<sessions[i].size(); j++){
//            cout << sessions[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << "Sessions size : " << sessions.size() << endl;
    for (int i = 0; i < numberOfPapers; ++i)
    {
        for (int j = 0; j < sessions.size(); ++j)
        {
            auto dist = 0.0;
            for (const auto &e : sessions[j])
            {
//                cout << "e : " << e << " ";
                dist += distanceMatrix[i][e];
//                cout << "dist : " << dist << " ";
            }
//            cout << endl;
            paperToSessionMatrix[i][j] = dist;
//            cout << "val : " << paperToSessionMatrix[i][j] << " ";

        }
//        cout << "i : " << i << endl;
    }
}

int* SimulatedAnnealing::randomInitialise() {
    int* randomState = new int[parallelSessions * maxPapersInSession];
//    std::iota(randomState, randomState+ parallelSessions * maxPapersInSession, 0);
    for(int i=0; i<numberOfPapers; i++){
        randomState[i] = i;
    }
    for(int i=numberOfPapers; i<parallelSessions*maxPapersInSession; i++){
        randomState[i] = -1;
    }
    std::shuffle(randomState, randomState+ parallelSessions * maxPapersInSession, randomEngine);
    return randomState;
}

SimulatedAnnealing::SimulatedAnnealing(double** inputDistanceMatrix, int parallelSessions, int maxPapersInSession, int tradeOffCoefficient, int numberOfPapers) {
    this->maxPapersInSession = maxPapersInSession;
    this->parallelSessions = parallelSessions;
    this->numberOfPapers = numberOfPapers;
    this->tradeOffCoefficient = tradeOffCoefficient;
    cout << "MPIS: " << maxPapersInSession << " PS: " << parallelSessions << " NOP: " << numberOfPapers << " TOC: " << tradeOffCoefficient << endl;
    initialiseDistanceMatrix(inputDistanceMatrix);
}

double SimulatedAnnealing::goodness(int *state) {
//    cout << "State : " << endl;
//    for(int i=0; i<maxPapersInSession*parallelSessions; i++){
//        cout << state[i] << " ";
//    }
//    cout << endl;

    double score1 = 0.0;
    for (int j = 0; j < parallelSessions; j++) {
        for (int k = 0; k < maxPapersInSession; k++) {
            for (int l = k + 1; l < maxPapersInSession; l++) {
                int index1 = j * (maxPapersInSession) + k;
                int index2 = j * (maxPapersInSession) + l;
                // Similarity between papers
                if (state[index1] != -1 && state[index2] != -1) {
                    score1 += 1 - distanceMatrix[state[index1]][state[index2]];
                }
            }
        }
    }
    // Sum of distances for competing papers.
//    for (int j = 0; j < parallelSessions; j++) {
//        for (int k = 0; k < maxPapersInSession; k++) {
//            for(int m = j+1; m < parallelSessions; m++) {
//                for (int l = k + 1; l < maxPapersInSession; l++) {
//                    int index1 = j * (parallelSessions) + k;
//                    int index2 = m * (parallelSessions) + l;
//                    // Similarity between papers
//                    score2 += distanceMatrix[state[index1]][state[index2]];
//                }
//            }
//        }
//    }
    double score2 = 0.0;
    for (int j = 0; j < parallelSessions; j++) {
        for (int k = j + 1; k < parallelSessions; k++) {
            for (int l = 0; l < maxPapersInSession; l++) {
                for (int m = 0; m < maxPapersInSession; m++) {
                    int index1 = j * (maxPapersInSession) + l;
                    int index2 = k * (maxPapersInSession) + m;
                    // Similarity between papers
                    if (state[index1] != -1 && state[index2] != -1) {
                        score2 += distanceMatrix[state[index1]][state[index2]];
                    }
                }
            }
        }
    }
//    cout << "Score 1 : " << score1 << " Score 2 : " << score2 << endl;

    double score = score1 + tradeOffCoefficient * score2;
    return score;
}
//double SimulatedAnnealing::goodness(int *state) {
//    double score1 = 0.0;
//
//    for (int j = 0; j < parallelSessions; j++) {
//        for (int k = 0; k < maxPapersInSession; k++) {
//            for (int l = k + 1; l < maxPapersInSession; l++) {
//                int index1 = j * (parallelSessions) + k;
//                int index2 = j * (parallelSessions) + l;
//                // Similarity between papers
//                if(state[index1]>=0&&state[index2]>=0&&state[index1]<numberOfPapers&&state[index2]<numberOfPapers)
//                {
//                    // std::cout<<"Similarity:"<<state[index1]<<" "<<state[index2]<<"\n";
//                    double temp=1 - distanceMatrix[state[index1]][state[index2]];
//                    score1 = score1 + temp;
//
//                }
//
//            }
//        }
//    }
//
//    // Sum of distances for competing papers.
//    double score2=0.0;
//    for(int i=0;i<parallelSessions;i++)
//    {
//        for(int j=0;j<maxPapersInSession;j++)
//        {
//            int index1=i*maxPapersInSession+j;
//            for(int k=(i+1)*maxPapersInSession;k<maxPapersInSession*parallelSessions;k++)
//            {
//                int index2=k;
//                if(state[index1]>=0&&state[index2]>=0&&state[index1]<numberOfPapers&&state[index2]<numberOfPapers)
//                {
//                    // std::cout<<index1<<" "<<k<<"\n";
//                    score2 += distanceMatrix[state[index1]][state[index2]];
//                }
//            }
//        }
//    }
//    double score = score1 +  score2;
//    return score;
//}



std::pair<int, int> SimulatedAnnealing::nextState() {
    int cnt = 0;
    int i, j;
    do
    {
        i = (int)uniformIntDistribution(randomEngine);
        j = (int)uniformIntDistribution(randomEngine);
        cnt += 1;
//    return score;
//}

    }
    // Randomly picking two papers from different sessions
    while (((i + maxPapersInSession) / maxPapersInSession) == ((j + maxPapersInSession) / maxPapersInSession) && cnt < 10);

    return std::make_pair(i, j);
}

double SimulatedAnnealing::changeInScore(int indexOne, int indexTwo, int *state) {
//    double change = 0;
//    int a = state[indexOne];
//    int b = state[indexTwo];
//    cout << "a b : " << a << " " << b << endl;
//    int session_seq_a = ((indexOne + maxPapersInSession) / maxPapersInSession) - 1;
//    int session_seq_b = ((indexTwo + maxPapersInSession) / maxPapersInSession) - 1;
//    if (session_seq_a == session_seq_b)
//        return 0;
//    cout << "hiyaa" << endl;
//    change = (tradeOffCoefficient + 1) * (paperToSessionMatrix[a][session_seq_a] + paperToSessionMatrix[b][session_seq_b]
//                                          - paperToSessionMatrix[a][session_seq_b] - paperToSessionMatrix[b][session_seq_a]
//                                          + 2 * distanceMatrix[a][b]);
//    return change;
    double currentScore = goodness(state);
    int a = state[indexOne];
    int b = state[indexTwo];
    state[indexOne] = b;
    state[indexTwo] = a;
    double newScore = goodness(state);
    state[indexOne] = a;
    state[indexTwo] = b;
    return newScore - currentScore;
}

int* SimulatedAnnealing::updateState(int indexOne, int indexTwo, int *state) {
    int a = state[indexOne];
    int b = state[indexTwo];
    int n = parallelSessions * maxPapersInSession;
    state[indexOne] = b;
    state[indexTwo] = a;
    int session_seq_a = ((indexOne + maxPapersInSession) / maxPapersInSession) - 1;
    int session_seq_b = ((indexTwo + maxPapersInSession) / maxPapersInSession) - 1;
//    cout << "here" << endl;
    for (int i = 0; i < numberOfPapers; ++i)
    {
//        cout << "a b : " << a << " " << b << endl;
        if(a != -1){
            paperToSessionMatrix[i][session_seq_a] -= distanceMatrix[i][a];
            paperToSessionMatrix[i][session_seq_b] += distanceMatrix[i][a];
//            cout << "In a ";
        }
        if(b != -1){
//            cout << "In b " << endl;
            paperToSessionMatrix[i][session_seq_a] += distanceMatrix[i][b];
            paperToSessionMatrix[i][session_seq_b] -= distanceMatrix[i][b];
//            cout << "In b " << endl;
        }
//        paperToSessionMatrix[i][session_seq_a] += distanceMatrix[i][b] - distanceMatrix[i][a];
//        paperToSessionMatrix[i][session_seq_b] += distanceMatrix[i][a] - distanceMatrix[i][b];
    }
//    cout << "dlfj";
}

int* SimulatedAnnealing::simulatedAnnealing(double duration) {
//    auto initial_time = Time::now();
//    decltype(initial_time) now;
//    decltype(now - initial_time) dur;
//    decltype(std::chrono::duration_cast<double_seconds>(dur)) secs;
    time_t start = time(0);
    randomEngine.seed(43);

    int *bestState = new int[maxPapersInSession*parallelSessions];
    double bestScore = 0;
//    cout << "duration : " << duration << endl;
    while(difftime(time(0), start) < duration){
//        cout << "secs count : " << secs.count() << endl;
        int* state = randomInitialise();
        initialisePaperToSessionMatrix(state);
//        cout << "Paper to session matrix : " << endl;
//        for(int i=0; i<numberOfPapers; i++){
//            for(int j=0; j<maxPapersInSession; j++){
//                cout << i << " " << j << endl;
//                cout << paperToSessionMatrix[i][j] << " ";
//            }
//            cout << endl;
//        }
//        int size = sizeof(state)/sizeof(state[0]); // Returning size as 2, check afterwards
//        cout << "State : " << endl;
//        for(int i=0; i<maxPapersInSession*parallelSessions; i++){
//            cout << state[i] << " ";
//        }
//        cout << endl;
        double initialScore = goodness(state);
//        cout << "Initial score : " << initialScore << endl;
        double totalChangeInScore = 0;
        int n = maxPapersInSession*parallelSessions;
        int countLimit = n;
        int ct = 0;
        while(ct < countLimit && difftime(time(0), start) < duration) {
            std::pair<int, int> next = nextState();
            int indexOne = (next.first%n + n)%n;
            int indexTwo = (next.second%n + n)%n;
//            cout << "Next state pair : " << indexOne << " " << indexTwo << endl;

            double changeInSkore = changeInScore(indexOne, indexTwo, state);
//            cout << "Change In Score : " << changeInSkore << endl;
            if(changeInSkore > 0){
                totalChangeInScore += changeInSkore;
                updateState(indexOne, indexTwo, state);
                ct=0;
            }
            else{
                double p = std::exp(changeInSkore * (ct + 1));
                bool update = static_cast<bool>(std::bernoulli_distribution(p)(randomEngine));

                if (update)
                {
                    totalChangeInScore += changeInSkore;
                    updateState(indexOne, indexTwo, state);
                }
            }
            if ((initialScore + totalChangeInScore) > bestScore)
            {
                cout << "Best score : " << bestScore << endl;
                bestScore = initialScore + totalChangeInScore;
                cout << "Best State : " << endl;
                for(int i=0; i<maxPapersInSession*parallelSessions; i++){
                    bestState[i] = state[i];
                    cout << bestState[i] << " ";
                }
                cout << endl;
            }
            ct++;
        }
    }
    cout << "Best score : " << bestScore << endl;
    cout << "Best state : ";
    for(int i=0; i<parallelSessions*maxPapersInSession; i++){
        cout << bestState[i] << " ";
    }
    return bestState;
}














