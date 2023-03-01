//
//  Graph.hpp
//
//  Created by Cameron Witkowski on 2021-07-21.
//

#ifndef Graph_h
#define Graph_h

#include <stdio.h>
#include <stdlib.h>

#include <tuple>
#include <vector>

#define POINTS 8000
#define BRAINWIDTH 125
#define COOLDOWN 5
#define OXYCOOLDOWN 50
#define OXYTHRESH 0.0001
#define OXYLAG 1
#define EQUILIBRIUM 0.15
#define GROWTH 1.00


// Branch death chemical signals?

// Reconsider long term stability? Can stable configurations unravel with bad luck? Is this natural?
// Growth is finding potential and SOMETHING else -> find this! Something with fibonacci perhaps?
// What's stopping organism from growing every tendril right close to its body? technically reduces potential the most.
// Pruning mechanism?
// localize dopamine and add gaba ?

using namespace std;

struct Edge {
    int pt;
    float weight;
    int fireRecency;
    float dist;
    struct Edge *reverseEdge;
} typedef Edge;

struct O2 {
    float amount;
    int age;
} typedef O2;

class Graph {
    vector< Edge > adjList[POINTS];
    vector< Edge > tempAdjList[POINTS];
    float activation[POINTS];
    float actStrength[POINTS];
    int sinceEating;
    int blockRecency[POINTS];
    O2 oxygen[POINTS];
    
    
    float kp = 2;
    float kr = 0.0000001;
    float ki = 0.00000000005;
    float kg = 0;
    
    int simCounter;
    
public:
    float food[POINTS];
    float potentialField[POINTS];
    float tempPotentialField[POINTS];
    float totalWeight[POINTS];


    float dopamine = 60;
    float hunger = 0;
    float reweight = 0;
    float reweightAccum = 0;
    float mass = 1000;
    float lrate = 0.6;
    
    Graph(int vertices, int w, float sparsity, float r) {
        int h = vertices / w;
        int i, j;
        for (i=0;i<vertices;i++) {
            for (j=0;j<vertices;j++) {
                if (i != j) {
                    float ijdist = (i%w - j%w)*(i%w - j%w) + (i/w - j/w)*(i/w - j/w);
                    if (ijdist/r < 5) {
                        addEdge(i, j, 1, ijdist);
                    }
                }
            }
            food[i] = 0;
            activation[i] = 99;
            oxygen[i] = {0, 99};
            blockRecency[i] = 9999;
            potentialField[i] = 0;
            totalWeight[i] = 0;
        }
        activation[0] = 0;
        sinceEating = 9999;
        
        // Set up reverse edge pointers
        for (i=0;i<vertices;i++) {
            for (j=0;j<adjList[i].size();j++) {
                // Find reverse edge
                int k;
                for (k=0;k<adjList[adjList[i][j].pt].size();k++) {
                    if (i == adjList[adjList[i][j].pt][k].pt) {
                        adjList[i][j].reverseEdge = &adjList[adjList[i][j].pt][k];
                        adjList[adjList[i][j].pt][k].reverseEdge = &adjList[i][j];
                        
                        tempAdjList[i][j].reverseEdge = &tempAdjList[tempAdjList[i][j].pt][k];
                        tempAdjList[tempAdjList[i][j].pt][k].reverseEdge = &tempAdjList[i][j];
                    }
                }
            }
        }
        
        oxygen[0] = {0, 0};
        simCounter = 0;
    }
    
    void addEdge(int u, int v, float weight, float dist) {
        if (u != v) {
            adjList[u].push_back(Edge{v, weight, 0, dist, 0});
            tempAdjList[u].push_back(Edge{v, weight, 0, dist, 0});
        }
    }
    
    vector<Edge> getNeighbours(int u) {
        return adjList[u];
    }
    
    void evolve() {
        float weights = 0;
        float liq02 = 0;
        for (int i=0;i<POINTS;i++) {
            liq02 += oxygen[i].amount;
            for (int j=0;j<adjList[i].size();j++) {
                weights += adjList[i][j].weight;
            }
        }

        
        printf("Total material: %f\n", weights);
        printf("Liq02: %f\n", liq02);
        printf("Dopamine: %.2f\n", dopamine);
        printf("Reweight: %f\n", reweight);
        
        int tempActivation[POINTS];
        O2 tempOxygen[POINTS];
        int numActivated = 0;
        
        simCounter += 1;
        
        if (activation[0] == 0) {
            oxygen[0].amount += 0;
            oxygen[0].age = 0;
            activation[0] = 9999;
            sinceEating = 0;
            printf("REACHED HOME BASE!!\n");
        }
        else {
            sinceEating += 1;
        }
        
        // Add 1 to activations
        for (int i=0;i<POINTS;i++) {
            tempActivation[i] = activation[i] + 1;
            tempOxygen[i] = oxygen[i];
            tempPotentialField[i] = potentialField[i];
            for (int j=0;j<adjList[i].size();j++) {
                tempAdjList[i][j].weight = adjList[i][j].weight;
            }
        }
        

        for (int i=0;i<POINTS;i++) {
            if (tempActivation[i] == 1) {
                float origWeight = 0;
                float addedWeight = 0;
                bool signalPassed = false;
                // Evolve activations
                for (int j=0;j<adjList[i].size();j++) {
                    float age = activation[adjList[i][j].pt];
                    float w = adjList[i][j].weight;
                    float r = float(rand())/float(RAND_MAX);
                    origWeight += w;
                    
                    // activation mechanism
                    if (r < w*dopamine*pow(age/(age+COOLDOWN), 1)) {
                        tempActivation[adjList[i][j].pt] = 0;
                        tempAdjList[i][j].fireRecency = simCounter;
                        
                        float newWeight = 1 - (1 - w)/(1.0 + lrate);
                        tempAdjList[i][j].weight = newWeight;
                        addedWeight += newWeight - w;
                        numActivated += 1;
                        signalPassed = true;
                    }
                    tempAdjList[i][j].weight = tempAdjList[i][j].weight / 1.2;
                }
                if (!signalPassed && float(rand())/float(RAND_MAX) < 0.05) {
                    float potI = 0;
                    for (int k=0;k<POINTS;k++) {
                        if (i != k) {
                            float x = (i%BRAINWIDTH - k%BRAINWIDTH);
                            float y = (i/BRAINWIDTH - k/BRAINWIDTH);

                            potI += (food[k] - totalWeight[k]) / sqrt(x*x + y*y);
                        }
                    }
                    for (int j=0;j<adjList[i].size();j++) {
                        float potEdge = 0;
                        for (int k=0;k<POINTS;k++) {
                            if (adjList[i][j].pt != k) {
                                float x = (adjList[i][j].pt%BRAINWIDTH - k%BRAINWIDTH);
                                float y = (adjList[i][j].pt/BRAINWIDTH - k/BRAINWIDTH);

                                potEdge += (food[k] - totalWeight[k]) / sqrt(x*x + y*y);
                            }
                        }
                        float prob = dopamine*(potEdge - potI)/weights;
                        
                        if (float(rand()) / float(RAND_MAX) < prob) {
                            printf("Added weight to (%d, %d), with prob: %f\n", i, adjList[i][j].pt, prob);
                            float w = tempAdjList[i][j].weight;
                            tempAdjList[i][j].weight = 1 - (1 - tempAdjList[i][j].weight) / 2;
                            mass += tempAdjList[i][j].weight - w;
                        }
                        if (float(rand()) / float(RAND_MAX) < prob + 0.01) {
                            printf("Removed weight to (%d, %d), with prob: %f\n", i, adjList[i][j].pt, prob);
                            float w = tempAdjList[i][j].reverseEdge->weight;
                            tempAdjList[i][j].reverseEdge->weight = w / 4;
                            mass += tempAdjList[i][j].reverseEdge->weight - w;
                        }
                    }
                }
                if (food[i] > 0) {
                    food[i] -= 1;
                }

            }
        }
        
        /*
        // evolve oxygen & update weights
        for (int i=0;i<POINTS;i++) {
            if (oxygen[i].amount > OXYTHRESH || true) {
                //printf("There is %2f oxygen at point: %d\n", oxygen[i].amount, i);
                //printf("   sinceEating: %d\n", sinceEating);
                if (activation[i] > 200) {
                    oxygen[i].amount = 0;
                }
                
                float recencySum = 0;
                
                // Calculate potential field
                for (int k=0;k<POINTS;k++) {
                    if (i != k) {
                        float x = (i%BRAINWIDTH - k%BRAINWIDTH);
                        float y = (i/BRAINWIDTH - k/BRAINWIDTH);

                        tempPotentialField[i] -= activation[i]/(activation[i] + COOLDOWN) / sqrt(x*x + y*y);
                    }
                }
                
                // Evolve o2
                for (int j=0;j<adjList[i].size();j++) {
                    int pt = adjList[i][j].pt;
                    
                    float x = (i%BRAINWIDTH - pt%BRAINWIDTH);
                    float y = (i/BRAINWIDTH - pt/BRAINWIDTH);
                                        
                    if (activation[adjList[i][j].pt] < 200) {
                        float grad = (potentialField[adjList[i][j].pt] - potentialField[i]) / sqrt(x*x + y*y);
                        
                        tempOxygen[adjList[i][j].pt].amount += oxygen[i].amount * 0.05 * (1.0 + 0.5/(1.0 + exp(-grad)));
                        tempOxygen[i].amount -= oxygen[i].amount * 0.05 * (1.0 + 0.5/(1.0 + exp(-grad)));
                        
                    }
                }
                if (float(rand())/float(RAND_MAX) < 0.001) {
                    printf("pot field %d, %f\n", i, potentialField[i]);
                }
            }
        }
         */
        
        // Prune unfiring edges
        for (int i=0;i<POINTS;i++) {
            for (int j=0;j<adjList[i].size();j++) {
                float fire = simCounter - adjList[i][j].fireRecency;
                //tempAdjList[i][j].weight = tempAdjList[i][j].weight/1.02;
                if (reweight < 0) {
                    tempAdjList[i][j].weight = tempAdjList[i][j].weight / (1 - reweight);
                }
                else {
                    tempAdjList[i][j].weight = 1 - (1 - tempAdjList[i][j].weight) / (1 + reweight);
                }
            }
        }
        
        for (int i=0;i<POINTS;i++) {
            activation[i] = tempActivation[i];
            oxygen[i] = tempOxygen[i];
            potentialField[i] = tempPotentialField[i];
            totalWeight[i] = 0;
            for (int j=0;j<adjList[i].size();j++) {
                adjList[i][j].weight = tempAdjList[i][j].weight;
                totalWeight[i] += adjList[i][j].weight;
            }
        }
        
    
        
        reweightAccum += mass - weights;
        dopamine += kp * (EQUILIBRIUM - float(numActivated)/float(mass));
        reweight = kr * (mass - weights) + ki * reweightAccum;
        
        lrate = lrate / 1.000;
        printf("lrate: %f\n", lrate);
        
        printf("NumActivations: %d\n", numActivated);

    }
    
    
    
    float *getActivation() {
        return activation;
    }
    
    O2 *getOxygen() {
        return oxygen;
    }
    
    void activate(int neuron) {
        if (simCounter > 500) {
            food[neuron] += 100;
            kg += 1;
        }
        else {
            activation[neuron] = 0;
        }
        
    }
    
    void feed() {
        
    }
    
};


#endif /* Graph_hpp */

