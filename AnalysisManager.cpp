//
// Created by Gianluca on 24/05/2019.
//

#include "AnalysisManager.h"

#include <iostream> //TODO vedi se tenere

AnalysisManager::AnalysisManager(std::shared_ptr<Solution> sol) : ProcessManager(std::move(sol)) {

}

AnalysisManager::~AnalysisManager() {
    //std::cout << "destroying AnalysisManager" << std::endl;
}

void AnalysisManager::operator()(int id) {
    runAnalysis();
    solution_->setObjectiveFunction(computeObjf());
}