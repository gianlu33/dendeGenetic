//
// Created by Gianluca on 25/05/2019.
//

#ifndef DENDEGENETIC_PROCESSMANAGER_H
#define DENDEGENETIC_PROCESSMANAGER_H

#include <memory>
#include <windows.h>
#include <string>

class Genetic;
class Solution;

class ProcessManager {
public:
    ProcessManager(std::shared_ptr<Solution> sol, Genetic &gen);
    ~ProcessManager();
    virtual void operator()(int id) = 0;
    static double computeObjf(std::string &folderName);


protected:
    void startProcess();
    double runAnalysis();
    void createDirectory();
    void cleanFS();
    void setFolderName(int id);
    void redirectOutputToNull();

    std::shared_ptr<Solution> solution_;
    PROCESS_INFORMATION pi_;
    STARTUPINFO si_;
    bool running_ = false;
    std::string folderName_;
    Genetic &gen_;
};


#endif //DENDEGENETIC_PROCESSMANAGER_H
