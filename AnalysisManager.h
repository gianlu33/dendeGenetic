//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_ANALYSISMANAGER_H
#define DENDEGENETIC_ANALYSISMANAGER_H

#include <memory>

#include "ProcessManager.h"

class Solution;
class Genetic;

class AnalysisManager : public ProcessManager {
public:
    explicit AnalysisManager(std::shared_ptr<Solution> sol, Genetic &gen);
    ~AnalysisManager();
    void operator()(int id) override;
};


#endif //DENDEGENETIC_ANALYSISMANAGER_H
