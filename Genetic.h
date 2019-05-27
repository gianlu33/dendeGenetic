//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_GENETIC_H
#define DENDEGENETIC_GENETIC_H

#include <string>
#include <vector>
#include <random>

#include "ProcessManager.h"
#include "AnalysisManager.h"
#include "LocalSearchManager.h"
#include "Solution.h"
#include "ctpl_stl.h"

class ProcessManager;
class AnalysisManager;
class LocalSearchManager;
class Solution;

class Genetic {
public:
    Genetic(int numPopulation, int nElite, char *output_file);
    Genetic(int numPopulation, int nElite, char *input_file, char *output_file);
    ~Genetic();

    void run();
    void test();

private:
    void addRandomSolutions(int number);
    std::array<bool, 20> generateRandomArray(std::mt19937 &gen, std::uniform_real_distribution<double> &dist);
    void sortPopulation();
    void runPool();
    std::shared_ptr<Solution> tournamentSelection();
    std::shared_ptr<Solution> crossover(std::shared_ptr<Solution> p1, std::shared_ptr<Solution> p2);
    void mutation(std::shared_ptr<Solution> child, double mutationRate);
    double getMutationRate(std::shared_ptr<Solution> p1, std::shared_ptr<Solution> p2);
    void initializeGenerator();

    const int numColumns_ = 8;
    int pressure_ = 2;
    int numPopulation_;
    int nElite_;
    int cpus_;
    char *output_file_;
    std::vector<std::shared_ptr<ProcessManager>> processManagers_;
    std::vector<std::shared_ptr<Solution>> population_;
    std::mt19937 randomGen_;
};


#endif //DENDEGENETIC_GENETIC_H
