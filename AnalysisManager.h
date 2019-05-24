//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_ANALYSISMANAGER_H
#define DENDEGENETIC_ANALYSISMANAGER_H

#include <windows.h>
#include <memory>
#include "Solution.h"

class Solution;

class AnalysisManager {
public:
    explicit AnalysisManager(std::shared_ptr<Solution> sol);
    ~AnalysisManager();
    void operator()(int id);


private:
    void startProcess();

    std::shared_ptr<Solution> solution_;
    PROCESS_INFORMATION pi_;
    STARTUPINFO si_;
    bool created_;

};


#endif //DENDEGENETIC_ANALYSISMANAGER_H
