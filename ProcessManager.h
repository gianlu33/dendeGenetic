//
// Created by Gianluca on 25/05/2019.
//

#ifndef DENDEGENETIC_PROCESSMANAGER_H
#define DENDEGENETIC_PROCESSMANAGER_H

#include <memory>
#include <windows.h>

#include "Solution.h"

class Solution;

class ProcessManager {
public:
    explicit ProcessManager(std::shared_ptr<Solution> sol);
    ~ProcessManager();
    virtual void operator()(int id) = 0;


protected:
    void startProcess();
    void runAnalysis();
    double computeObjf();

    std::shared_ptr<Solution> solution_;
    PROCESS_INFORMATION pi_;
    STARTUPINFO si_;
    bool running_;
};


#endif //DENDEGENETIC_PROCESSMANAGER_H
