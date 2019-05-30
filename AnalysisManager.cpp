//
// Created by Gianluca on 24/05/2019.
//

#include "AnalysisManager.h"
#include "ProcessManager.h"
#include "Genetic.h"
#include "Solution.h"

#include <iostream>

AnalysisManager::AnalysisManager(std::shared_ptr<Solution> sol, Genetic &gen) :
    ProcessManager(sol, gen) {

}

AnalysisManager::~AnalysisManager() {
    //std::cout << "destroying AnalysisManager" << std::endl;
}

void AnalysisManager::operator() (int id) {
    setFolderName(id);

    try {
        createDirectory();
        auto objf = runAnalysis();
        solution_->setObjectiveFunction(objf);
        gen_.checkAndSetBestSolution(solution_);
        std::cout << "[" << id << "] analysis completed: " << objf << std::endl;
    }
    catch(...){
        return;
    }


}