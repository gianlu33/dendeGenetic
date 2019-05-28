//
// Created by Gianluca on 24/05/2019.
//

#include "ProcessManager.h"
#include <string>
#include <windows.h>
#include <sstream>
#include <iostream> //TODO vedere se tenere
#include <cmath>

#include "GeneticException.h"
#include "IOManager.h"

extern char programName[50];

ProcessManager::ProcessManager(std::shared_ptr<Solution> sol) {
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
}

void ProcessManager::startProcess(std::string &folderName) {
    std::stringstream name;
    name << programName <<  " " << folderName << " " << solution_->getArrayString();

    char nameChar[200];
    strcpy(nameChar, name.str().c_str());


    ZeroMemory( &si_, sizeof(si_) );
    si_.cb = sizeof(si_);
    ZeroMemory( &pi_, sizeof(pi_) );

    redirectOutputToNull(folderName);

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
        throw GeneticException();
}

double ProcessManager::runAnalysis(std::string folderName) {
    DWORD returnValue;

    startProcess(folderName);
    WaitForSingleObject( pi_.hProcess, INFINITE );

    int exitcode = GetExitCodeProcess(pi_.hProcess, &returnValue);
    CloseHandle( pi_.hProcess );
    CloseHandle( pi_.hThread );
    CloseHandle(si_.hStdError);
    running_ = false;

    if(!exitcode || returnValue < 0)
        throw GeneticException();

    return computeObjf(folderName);
}

double ProcessManager::computeObjf(std::string folderName) {
    //read from file and compute objf value
    std::vector<double> values;

    //read from output files
    for(int i=0; i<5; i++){
        std::string filename = folderName + "/" + std::to_string(i) + ".txt";
        try {
            values.push_back(IOManager::getMaxAbsValue(filename));
        }
        catch(...){
            throw GeneticException();
        }
    }

    double tot = 0;
    for(double &val : values)
        tot += std::pow(val, 2);

    return std::sqrt(tot);
}

void ProcessManager::createDirectory(std::string &folderName){
    if(!CreateDirectoryA(folderName.c_str(), nullptr))
        throw GeneticException();
}

void ProcessManager::cleanFS(std::string &folderName) {
    for(int i=0; i<5; i++){
        std::string name = folderName + "/" + std::to_string(i) + ".txt";
        DeleteFileA(name.c_str());
    }

    std::string nullfile = folderName + "/null";
    DeleteFileA(nullfile.c_str());

    RemoveDirectoryA(folderName.c_str());
}

std::string ProcessManager::getFolderName(int id){
    return "__folder" + std::to_string(id) + "__";
}

void ProcessManager::redirectOutputToNull(std::string &folderName){
    std::string filename = folderName + "/null";
    char namech[50];

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