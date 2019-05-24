//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_GENETIC_H
#define DENDEGENETIC_GENETIC_H

#include <string>
#include <vector>

#include "AnalysisManager.h"
#include "Solution.h"

class AnalysisManager;
class Solution;

class Genetic {
public:
    Genetic(int numPopulation, char *output_file);
    Genetic(int numPopulation, char *input_file, char *output_file);
    ~Genetic();

    void run();

private:
    int numPopulation_;
    int cpus_;
    char *output_file_;
    std::vector<std::shared_ptr<AnalysisManager>> analysisManagers_;
    std::vector<std::shared_ptr<Solution>> solutions_;
};


#endif //DENDEGENETIC_GENETIC_H
