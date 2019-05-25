//
// Created by Gianluca on 24/05/2019.
//
#include <vector>
#include <memory>
#include <thread>
#include <iostream>

#include "ctpl_stl.h"
#include "Genetic.h"
#include "AnalysisManager.h"

Genetic::Genetic(int numPopulation, char *output_file) {
    cpus_ = std::thread::hardware_concurrency();
    numPopulation_ = numPopulation;
}

Genetic::Genetic(int numPopulation, char *input_file, char *output_file) {

}

Genetic::~Genetic() {
    //empty vectors of managers
    std::cout << "destroying Genetic" << std::endl;
    processManagers_.clear();
}

void Genetic::run() {

    //initialize solutions..
    for(int i=0; i<numPopulation_; i++){
        auto sol = std::make_shared<Solution>();
        processManagers_.push_back(std::make_shared<AnalysisManager>(sol));
        solutions_.push_back(sol);
    }

    //thread pools
    ctpl::thread_pool p(cpus_);

    for(auto& man : processManagers_){
        p.push(std::ref(*man));
    }

    std::cout << "Waiting.." << std::endl;
    p.stop(true);
    std::cout << "Finished" << std::endl;
    processManagers_.clear();

}
