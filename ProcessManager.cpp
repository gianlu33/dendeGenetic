//
// Created by Gianluca on 24/05/2019.
//

#include "ProcessManager.h"
#include <cstring>
#include <string>
#include <iostream>
#include <random>
#include <time.h>

extern char programName[20];

ProcessManager::ProcessManager(std::shared_ptr<Solution> sol) {
    solution_ = std::move(sol);
}

ProcessManager::~ProcessManager() {
    //std::cout << "destroying ProcessManager" << std::endl;

    if(running_){
        TerminateProcess(pi_.hProcess, 0);
        CloseHandle( pi_.hProcess );
        CloseHandle( pi_.hThread );
    }
}

void ProcessManager::startProcess() {

    ZeroMemory( &si_, sizeof(si_) );
    si_.cb = sizeof(si_);
    ZeroMemory( &pi_, sizeof(pi_) );

    running_ = CreateProcess(nullptr,   // No module name (use command line)
                             programName,        // Command line
                             nullptr,           // Process handle not inheritable
                             nullptr,           // Thread handle not inheritable
                             FALSE,          // Set handle inheritance to FALSE
                             0,              // No creation flags
                             nullptr,           // Use parent's environment block
                             nullptr,           // Use parent's starting directory
                             &si_,            // Pointer to STARTUPINFO structure
                             &pi_ ) != 0;
}

void ProcessManager::runAnalysis() {
    startProcess();

    WaitForSingleObject( pi_.hProcess, INFINITE );
    CloseHandle( pi_.hProcess );
    CloseHandle( pi_.hThread );
    running_ = false;
}

double ProcessManager::computeObjf() {
    //read from file and update objf value
    //TODO togliere random e mettere vera analisi
    auto dist = std::uniform_real_distribution<double>(0,50000);
    auto dev = std::random_device();
    auto gen = std::mt19937{dev()};
    gen.seed(time(NULL));

    return dist(gen);
}