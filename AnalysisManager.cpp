//
// Created by Gianluca on 24/05/2019.
//

#include "AnalysisManager.h"
#include <cstring>
#include <string>
#include <iostream>

AnalysisManager::AnalysisManager(std::shared_ptr<Solution> sol) : ProcessManager(std::move(sol)) {

}

AnalysisManager::~AnalysisManager() {
    //std::cout << "destroying AnalysisManager" << std::endl;
}

void AnalysisManager::operator()(int id) {
    startProcess();

    WaitForSingleObject( pi_.hProcess, INFINITE );
    CloseHandle( pi_.hProcess );
    CloseHandle( pi_.hThread );
    running_ = false;

    //read from file and update objf value
}