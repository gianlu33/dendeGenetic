//
// Created by Gianluca on 24/05/2019.
//

#include "ProcessManager.h"
#include <windows.h>
#include <sstream>
#include <iostream>
#include <cmath>

#include "IOManager.h"
#include "Utils.h"
#include "Genetic.h"

#include <thread>
#include <chrono>

extern char programName[50];

ProcessManager::ProcessManager(std::shared_ptr<Solution> sol, Genetic &gen) :
    gen_(gen){
    solution_ = std::move(sol);
}

ProcessManager::~ProcessManager() {
    //std::cout << "destroying ProcessManager" << std::endl;
    if(running_){
        TerminateProcess(pi_.hProcess, 0);
        CloseHandle( pi_.hProcess );
        CloseHandle( pi_.hThread );
        CloseHandle(si_.hStdError);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    if(!folderName_.empty())
        cleanFS();
}

void ProcessManager::startProcess() {
    std::stringstream name;
    name << programName <<  " " << folderName_ << " " << solution_->getArrayString();

    char nameChar[400];
    strcpy(nameChar, name.str().c_str());

    ZeroMemory( &si_, sizeof(si_) );
    si_.cb = sizeof(si_);
    ZeroMemory( &pi_, sizeof(pi_) );

    redirectOutputToNull();

    running_ = CreateProcess(nullptr,   // No module name (use command line)
                             nameChar,        // Command line
                             nullptr,           // Process handle not inheritable
                             nullptr,           // Thread handle not inheritable
                             FALSE,          // Set handle inheritance to FALSE
                             0,              // No creation flags
                             nullptr,           // Use parent's environment block
                             nullptr,           // Use parent's starting directory
                             &si_,            // Pointer to STARTUPINFO structure
                             &pi_ ) != 0;

    if(!running_)
        throw utils::GeneticException();

}

double ProcessManager::runAnalysis() {
    DWORD returnValue;

    startProcess();
    WaitForSingleObject( pi_.hProcess, INFINITE );

    int exitcode = GetExitCodeProcess(pi_.hProcess, &returnValue);
    CloseHandle( pi_.hProcess );
    CloseHandle( pi_.hThread );
    CloseHandle(si_.hStdError);
    running_ = false;

    if(gen_.stop_ || !exitcode || returnValue != 0) {
        //std::cout << "Failed" << std::endl;
        throw utils::GeneticException();
    }
    return computeObjf(folderName_);
}

double ProcessManager::computeObjf(std::string &folderName) {
    //read from file and compute objf value
    std::vector<double> values;
    std::vector<double> maxValues {0.021, 0.007, 0.005, 0.004, 0.002};

    //read from output files
    for(int i=0; i<5; i++){
        std::string filename = folderName + "/" + std::to_string(i) + ".txt";
        try {
            values.push_back(IOManager::getMaxAbsValue(filename));
        }
        catch(...){
            throw utils::GeneticException();
        }
    }

    double tot = 0;
    for(int i=0; i<5; i++){
        auto val = values.at(i);
        auto maxVal = maxValues.at(i);

        if(val >= maxVal)
            throw utils::GeneticException();

        tot += std::pow(val, 2);
    }

    //TODO vedi se aggiungere un moltiplicatore per tener conto del num di colonne
    return std::sqrt(tot);
}

void ProcessManager::createDirectory(){
    if(!CreateDirectoryA(folderName_.c_str(), nullptr))
        throw utils::GeneticException();
}

void ProcessManager::cleanFS() {
    for(int i=0; i<5; i++){
        std::string name = folderName_ + "/" + std::to_string(i) + ".txt";
        DeleteFileA(name.c_str());
    }

    std::string nullfile = folderName_ + "/null";
    DeleteFileA(nullfile.c_str());

    RemoveDirectoryA(folderName_.c_str());
}

void ProcessManager::setFolderName(int id){
    folderName_ = "__folder" + std::to_string(id) + "__";
}

void ProcessManager::redirectOutputToNull(){
    std::string filename = folderName_ + "/null";
    char namech[100];

    strcpy(namech, filename.c_str());

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE h = CreateFile(namech,
                          FILE_APPEND_DATA,
                          FILE_SHARE_WRITE | FILE_SHARE_READ,
                          &sa,
                          OPEN_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL );

    si_.cb = sizeof(STARTUPINFO);
    si_.dwFlags |= STARTF_USESTDHANDLES;
    si_.hStdInput = NULL;
    si_.hStdError = h;
    si_.hStdOutput = h;
}