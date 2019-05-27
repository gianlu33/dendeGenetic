//
// Created by Gianluca on 24/05/2019.
//

#include "LocalSearchManager.h"
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include <time.h>

#include "Utils.h"

LocalSearchManager::LocalSearchManager(std::shared_ptr<Solution> sol, std::mt19937 &randomGen) :
    ProcessManager(std::move(sol)),
    randomGen_(randomGen){}

LocalSearchManager::~LocalSearchManager() {
    //std::cout << "destroying LocalSearchManager" << std::endl;
}

void LocalSearchManager::operator()(int id) {
    //first improvement: create random ordered sequence from 0 to 20
    //and according to that ordering, change the value of the index in array
    //after that, recompute the objf, and if it's an improvement "jump" to that solution, and repeat
    auto indexes = utils::genIndexedVector(20);

    //TODO vedi la durata di questo while
    while(true){
        std::shuffle(indexes.begin(), indexes.end(), randomGen_);
        auto objf = solution_->getObjectiveFunction();

        for(int i=0; i<20; i++){
            //change value
            solution_->flip(indexes[i]);

            //analysis..
            runAnalysis();
            auto newObjf = computeObjf();

            if(newObjf < objf){
                solution_->setObjectiveFunction(newObjf);
                break;
            }

            //no improvement, restore index
            solution_->flip(indexes[i]);
        }

        //if no improvement at all, exit
        if(objf == solution_->getObjectiveFunction())
            break;
    }

}

