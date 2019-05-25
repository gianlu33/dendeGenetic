//
// Created by Gianluca on 24/05/2019.
//

#include "ProcessManager.h"
#include <cstring>
#include <string>
#include <iostream>

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