//
// Created by Gianluca on 24/05/2019.
//
#include <memory>
#include <thread>
#include <iostream>
#include <chrono>
#include <time.h>

#include "ctpl_stl.h"
#include "Genetic.h"
#include "AnalysisManager.h"
#include "IOManager.h"

Genetic::Genetic(int numPopulation, char *output_file) {
    cpus_ = std::thread::hardware_concurrency();
    numPopulation_ = numPopulation;
    output_file_ = output_file;

    //create random solutions
    addRandomSolutions(numPopulation);

    //sort array
    sortArray();
}

Genetic::Genetic(int numPopulation, char *input_file, char *output_file) {
    cpus_ = std::thread::hardware_concurrency();
    numPopulation_ = numPopulation;
    output_file_ = output_file;

    //initialize solutions
    population_ = IOManager::readInput(input_file);

    //if population is more than numPopulation, delete the worst solutions
    while(population_.size() > numPopulation)
        population_.pop_back();

    //if population is less than numPopulation, add random solutions
    if(population_.size() < numPopulation)
        addRandomSolutions(numPopulation - population_.size());

    //sort array (not necessary if no random solutions are created, but do it anyway)
    sortArray();
}

Genetic::~Genetic() {
    //empty vectors of managers and write population to file
    std::cout << "destroying Genetic" << std::endl;
    processManagers_.clear();

    //sort array (not necessary, but do it anyway)
    sortArray();

    //write to output file
    IOManager::writeOutput(output_file_, population_);
}

void Genetic::run() {
    /* TODO
    //initialize solutions..
    for(int i=0; i<numPopulation_; i++){
        auto sol = std::make_shared<Solution>();
        processManagers_.push_back(std::make_shared<AnalysisManager>(sol));
        population_.push_back(sol);
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

     */

}

void Genetic::addRandomSolutions(int number) {
    std::vector<std::shared_ptr<Solution>> newSols;

    auto dev = std::random_device();
    auto gen = std::mt19937{dev()};
    //TODO vedi seed
    gen.seed(time(NULL));
    auto dist = std::uniform_real_distribution<double>(0,1);

    //TODO da togliere random objf e mettere poi analisi
    auto devTMP = std::random_device();
    auto genTMP = std::mt19937{dev()};
    auto distTMP = std::uniform_real_distribution<double>(0,50000);

    for(int i=0; i<number; i++)
        newSols.push_back(std::make_shared<Solution>(distTMP(genTMP), generateRandomArray(gen, dist)));

    //analysis..

    //push to population
    for(auto &sol : newSols)
        population_.push_back(sol);
}

std::array<bool, 20> Genetic::generateRandomArray(std::mt19937 &gen, std::uniform_real_distribution<double> &dist) {
    std::array<bool, 20> array{};
    double probability = numColumns_ / 20.0;

    for(int i=0; i<20; i++){
        array[i] = dist(gen) <= probability;
    }

    return array;
}

void Genetic::sortArray() {
    std::sort(population_.begin(), population_.end(),
              [](std::shared_ptr<Solution> a,std::shared_ptr<Solution> b)
                {return b->getObjectiveFunction() < a->getObjectiveFunction();});
}