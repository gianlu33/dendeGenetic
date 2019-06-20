//
// Created by Gianluca on 24/05/2019.
//

#include "LocalSearchManager.h"
#include <iostream>
#include <algorithm>

#include "Utils.h"
#include "Genetic.h"
#include "ProcessManager.h"
#include "Solution.h"

LocalSearchManager::LocalSearchManager(std::shared_ptr<Solution> sol, Genetic &gen, std::mt19937 &randomGen) :
    ProcessManager(sol, gen),
    randomGen_(randomGen){}

LocalSearchManager::~LocalSearchManager() {
    //std::cout << "destroying LocalSearchManager" << std::endl;
}

void LocalSearchManager::operator()(int id) {
    setFolderName(id);
    auto solTemp = std::make_shared<Solution>(solution_->getObjectiveFunction(), solution_->getArray(), solution_->isFeasible());

    //first improvement: create random ordered sequence from 0 to 20
    //and according to that ordering, change the value of the index in array
    //after that, recompute the objf, and if it's an improvement "jump" to that solTemp, and repeat
    auto indexes = utils::genIndexedVector(20);
    std::pair<double, bool> pair;

    try {
        createDirectory();
    }
    catch(...){
        return;
    }

    for(int nRuns=0; nRuns<RUNS; nRuns++){
        std::shuffle(indexes.begin(), indexes.end(), randomGen_);
        auto objf = solTemp->getObjectiveFunction();

        for(int i=0; i<ITERATIONS; i++){
            //change value
            solTemp->flip(indexes[i]);

            //analysis..
            try {
                pair = runAnalysis();
                //std::cout << "[" << id << "] analysis completed: " << pair.first << " " << pair.second << std::endl;
            }
            catch(...){
                //std::cout << "[" << id << "] analysis failed" << std::endl;
                solTemp->flip(indexes[i]);
                continue;
            }

            if(pair.first < objf){
                solTemp->setObjectiveFunction(pair.first);
                solTemp->setFeasible(pair.second);
                gen_.checkAndSetBestSolution(solTemp);
                break;
            }

            //no improvement, restore index
            solTemp->flip(indexes[i]);
        }

        //if no improvement at all, exit
        if(objf == solTemp->getObjectiveFunction())
            break;
    }

    solution_->setFeasible(solTemp->isFeasible());
    solution_->setObjectiveFunction(solTemp->getObjectiveFunction());
    solution_->setArray(solTemp->getArray());
}

