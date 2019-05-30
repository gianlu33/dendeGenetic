//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_LOCALSEARCHMANAGER_H
#define DENDEGENETIC_LOCALSEARCHMANAGER_H

#include "ProcessManager.h"
#include <random>

class Genetic;
class Solution;

class LocalSearchManager : public ProcessManager {
public:
    LocalSearchManager(std::shared_ptr<Solution> sol, Genetic &gen, std::mt19937 &randomGen);
    ~LocalSearchManager();
    void operator()(int id) override;

private:
    std::mt19937 &randomGen_;
};


#endif //DENDEGENETIC_LOCALSEARCHMANAGER_H
