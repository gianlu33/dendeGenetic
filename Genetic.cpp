//
// Created by Gianluca on 24/05/2019.
//
#include <memory>
#include <thread>
#include <iostream>
#include <chrono>
#include <time.h>
#include <sstream>

#include "Genetic.h"
#include "AnalysisManager.h"
#include "IOManager.h"
#include "Utils.h"

Genetic::Genetic(int numPopulation, int nElite, char *output_file) {
    cpus_ = std::thread::hardware_concurrency();
    numPopulation_ = numPopulation;
    nElite_ = nElite;
    output_file_ = output_file;

    initializeGenerator();

    //create random solutions
    addRandomSolutions(numPopulation);

    //sort array
    sortPopulation();
}

Genetic::Genetic(int numPopulation, int nElite, char *input_file, char *output_file) {
    cpus_ = std::thread::hardware_concurrency();
    numPopulation_ = numPopulation;
    nElite_ = nElite;
    output_file_ = output_file;

    initializeGenerator();

    //initialize solutions
    try {
        population_ = IOManager::readInput(input_file);
    }
    catch(...){
        std::cout << "Error in reading from input file. Creating random solutions.." << std::endl;
        population_.clear();
        addRandomSolutions(numPopulation);
    }

    //if population is more than numPopulation, delete the worst solutions
    while(population_.size() > numPopulation)
        population_.pop_back();

    //if population is less than numPopulation, add random solutions
    if(population_.size() < numPopulation)
        addRandomSolutions(numPopulation - static_cast<int>(population_.size()));

    //sort array (not necessary if no random solutions are created, but do it anyway)
    sortPopulation();
}

Genetic::~Genetic() {
    //empty vectors of managers and write population to file
    std::cout << "destroying Genetic" << std::endl;
    processManagers_.clear();

    //sort array (not necessary, but do it anyway)
    sortPopulation();

    //write to output file
    try {
        IOManager::writeOutput(output_file_, population_);
    }
    catch(...){
        std::stringstream ss;

        ss << "Error in writing to output file." << std::endl;
        ss << "The best solution found so far is:" << std::endl;
        ss << population_[0]->to_string() << std::endl;
        ss << "Save it!" << std::endl;

        std::cout << ss.str();
    }
}

void Genetic::run() {
    int numGeneration = 0;
    std::vector<std::shared_ptr<Solution>> children;

    while(true){
        std::cout << "Starting generation " << numGeneration << std::endl;

        //TODO elitism ?
        for(int i=0; i<nElite_; i++)
            children.push_back(population_[i]);

        for(int i=0; i<numPopulation_ - nElite_; i++){
            //tournament selection
            auto parent1 = tournamentSelection();
            auto parent2 = tournamentSelection();

            //combination (crossover, mutation, etc)
            auto child = crossover(parent1, parent2);
            mutation(child, getMutationRate(parent1, parent2));

            //add to children
            children.push_back(child);
        }

        //compute objf
        std::cout << "Computing objective function for children" << std::endl;
        for(auto child : children)
            processManagers_.push_back(std::make_shared<AnalysisManager>(child));
        runPool();

        //local search
        std::cout << "Starting local search" << std::endl;
        for(auto child : children)
            processManagers_.push_back(std::make_shared<LocalSearchManager>(child, randomGen_));
        runPool();

        //new population
        population_ = children;
        children.clear();

        //sort
        sortPopulation();

        pressure_++;

        std::cout << "Generation " << numGeneration << " ended" << std::endl;
        std::cout << "Best solution until now:" << std::endl;
        std::cout << population_[0]->to_string() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(20));
    }
}

void Genetic::addRandomSolutions(int number) {
    std::vector<std::shared_ptr<Solution>> newSols;

    auto dist = std::uniform_real_distribution<double>(0,1);

    //TODO da togliere random objf e mettere poi analisi
    auto distTMP = std::uniform_real_distribution<double>(0,50000);

    for(int i=0; i<number; i++)
        newSols.push_back(std::make_shared<Solution>(distTMP(randomGen_), generateRandomArray(randomGen_, dist)));

    //analysis..
    for(int i=0; i<number; i++){
        processManagers_.push_back(std::make_shared<AnalysisManager>(newSols[i]));
    }
    runPool();

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

void Genetic::sortPopulation() {
    std::sort(population_.begin(), population_.end(),
              [](std::shared_ptr<Solution> a,std::shared_ptr<Solution> b)
                {return a->getObjectiveFunction() < b->getObjectiveFunction();});
}

void Genetic::runPool(){
    ctpl::thread_pool pool(cpus_);

    for(auto& man : processManagers_){
        pool.push(std::ref(*man));
    }

    std::cout << "Running threads.." << std::endl;
    pool.stop(true);
    std::cout << "Finished" << std::endl;
    processManagers_.clear();
    pool.clear_queue();
}

std::shared_ptr<Solution> Genetic::tournamentSelection() {
    auto indexes = utils::genIndexedVector(numPopulation_);
    std::shuffle(indexes.begin(), indexes.end(), randomGen_);

    int best = -1;
    for(int i=0; i<pressure_; i++){
        int value = indexes[i];
        if(best == -1 || value < best)
            best = value;
    }

    return population_[best];
}

std::shared_ptr<Solution> Genetic::crossover(std::shared_ptr<Solution> p1, std::shared_ptr<Solution> p2) {
    auto dist = std::uniform_int_distribution<int>(0,1);

    std::array<bool, 20> array{};

    for(int i=0; i<20; i++){
        if(dist(randomGen_)){
            array[i] = p1->getArrayValue(i);
        }
        else {
            array[i] = p2->getArrayValue(i);
        }
    }

    return std::make_shared<Solution>(array);
}

void Genetic::mutation(std::shared_ptr<Solution> child, double mutationRate) {
    auto dist = std::uniform_real_distribution<double>(0,1);

    for(int i=0; i<20; i++){
        if(dist(randomGen_) < mutationRate)
            child->flip(i);
    }
}

double Genetic::getMutationRate(std::shared_ptr<Solution> p1, std::shared_ptr<Solution> p2) {
    int equalGenes = 0;
    int totalGenes = 0;

    for(int i=0; i<20; i++){
        if(p1->getArrayValue(i) && p2->getArrayValue(i))
            equalGenes++;

        if(p1->getArrayValue(i) || p2->getArrayValue(i))
            totalGenes++;
    }

    double similarity = ((double) equalGenes) / totalGenes;

    //std::cout << "EqualGenes: " << equalGenes << std::endl;
    //std::cout << "TotalGenes: " << totalGenes << std::endl;
    //std::cout << "Similarity: " << similarity << std::endl;

    return similarity * 0.80; //TODO vedere questo valore
}

void Genetic::test() {
    //test tournament selection
    auto par1 = tournamentSelection();
    auto par2 = tournamentSelection();

    std::cout << "Extracted:" << std::endl;
    std::cout << par1->to_string() << std::endl;
    std::cout << par2->to_string() << std::endl;

    auto child = crossover(par1, par2);

    std::cout << "Child:" << std::endl;
    std::cout << child->to_string() << std::endl;

    auto mRate = getMutationRate(par1, par2);

    std::cout << "Mutation rate: " << mRate << std::endl;

    mutation(child, mRate);
    std::cout << "Child mutated:" << std::endl;
    std::cout << child->to_string() << std::endl;
}

void Genetic::initializeGenerator(){
    auto dev = std::random_device();
    randomGen_ = std::mt19937(dev());
    //TODO vedi seed
    randomGen_.seed(time(NULL));
}