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

class ProcessManager;
class AnalysisManager;
class LocalSearchManager;
class Solution;

class Genetic {
public:
    Genetic(int numPopulation, char *output_file);
    Genetic(int numPopulation, char *input_file, char *output_file);
    ~Genetic();

    void run();

private:
    void addRandomSolutions(int number);
    std::array<bool, 20> generateRandomArray(std::mt19937 &gen, std::uniform_real_distribution<double> &dist);
    void sortArray();

    const int numColumns_ = 8;
    int numPopulation_;
    int cpus_;
    char *output_file_;
    std::vector<std::shared_ptr<ProcessManager>> processManagers_;
    std::vector<std::shared_ptr<Solution>> population_;
};


#endif //DENDEGENETIC_GENETIC_H
