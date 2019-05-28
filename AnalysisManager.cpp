//
// Created by Gianluca on 24/05/2019.
//

#include "AnalysisManager.h"

#include <iostream> //TODO vedi se tenere

AnalysisManager::AnalysisManager(std::shared_ptr<Solution> sol) : ProcessManager(sol) {

}

AnalysisManager::~AnalysisManager() {
    //std::cout << "destroying AnalysisManager" << std::endl;
}

void AnalysisManager::operator()(int id) {
    auto folderName = getFolderName(id);

    try {
        createDirectory(folderName);
        auto objf = runAnalysis(folderName);
        solution_->setObjectiveFunction(objf);
        cleanFS(folderName);
    }
    catch(...){
        cleanFS(folderName);
        return;
    }


}