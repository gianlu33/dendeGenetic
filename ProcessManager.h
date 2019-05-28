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
    void startProcess(std::string &folderName);
    double runAnalysis(std::string folderName);
    static double computeObjf(std::string folderName);
    static void createDirectory(std::string &folderName);
    static void cleanFS(std::string &folderName);
    static std::string getFolderName(int id);
    void redirectOutputToNull(std::string &folderName);

        std::shared_ptr<Solution> solution_;
    PROCESS_INFORMATION pi_;
    STARTUPINFO si_;
    bool running_ = false;
};


#endif //DENDEGENETIC_PROCESSMANAGER_H
