//
// Created by Gianluca on 24/05/2019.
//
#include <memory>
#include <thread>
#include <iostream>
#include <time.h>
#include <sstream>
#include <cmath>
#include <ctime>
#include <chrono>

#include "Genetic.h"
#include "AnalysisManager.h"
#include "IOManager.h"
#include "Utils.h"

Genetic::Genetic(int numPopulation, int nElite, char *output_file) {
    cpus_ = std::thread::hardware_concurrency();
    numPopulation_ = numPopulation;
    nElite_ = nElite;
    output_file_ = output_file;
    bestSolution_ = std::make_shared<Solution>();
    stop_ = false;

    initializeGenerator();
}

Genetic::Genetic(int numPopulation, int nElite, char *input_file, char *output_file) {
    cpus_ = std::thread::hardware_concurrency();
    numPopulation_ = numPopulation;
    nElite_ = nElite;
    output_file_ = output_file;
    bestSolution_ = std::make_shared<Solution>();
    stop_ = false;

    initializeGenerator();

    //initialize solutions
    try {
        population_ = IOManager::readInput(input_file);
    }
    catch(...){
        std::cout << "Error in reading from input file. Continuing with a new run" << std::endl;
        population_.clear();
        return;
    }

    if(!population_.empty())
        checkAndSetBestSolution(population_[0]);

    //if population is more than numPopulation, delete the worst solutions
    while(population_.size() > numPopulation)
        population_.pop_back();
}

Genetic::~Genetic() {
    //empty vectors of managers and write population to file
    stop_ = true;
    processManagers_.clear();

    if(!init_) return;

    //sort array (not necessary, but do it anyway)
    sortPopulation();

    //add best solution if not yet present
    auto bestSol = getBestSolution();
    if(population_[0]->getObjectiveFunction() > bestSol->getObjectiveFunction()){
        population_.insert(population_.begin(), bestSol);
        population_.pop_back();
    }

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

void Genetic::init(){
    if(population_.size() >= numPopulation_){
       init_ = true;
       return;
    }

    auto timenow =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::cout << ctime(&timenow) << "Initializing.." << std::endl;

    //create random solutions
    addRandomSolutions(numPopulation_ - static_cast<int>(population_.size()));

    //sort array
    sortPopulation();

    init_ = true;

    timenow =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::cout << ctime(&timenow) << "Initial population generated" << std::endl;
    std::cout << "Sleeping for 30 sec.." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(30));
}

void Genetic::run() {
    if(population_.empty()){
        std::cout << "You must call the init() method first!" << std::endl;
        return;
    }

    int numGeneration = 0;
    std::vector<std::shared_ptr<Solution>> children;

    while(true){
        auto timenow =
                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        std::cout << ctime(&timenow) << "Starting generation " << numGeneration << std::endl;

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
            processManagers_.push_back(std::make_shared<AnalysisManager>(child, *this));
        runPool();

        //local search
        timenow =
                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        std::cout << ctime(&timenow) << "Starting local search" << std::endl;
        for(auto child : children)
            processManagers_.push_back(std::make_shared<LocalSearchManager>(child, *this, randomGen_));
        runPool();

        //new population
        population_ = children;
        children.clear();

        //sort
        sortPopulation();

        pressure_++;

        timenow =
                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        std::cout << ctime(&timenow) << "Generation " << numGeneration << " ended" << std::endl;
        std::cout << "Best solution until now:" << std::endl;
        std::cout << population_[0]->to_string() << std::endl;

        std::cout << "Sleeping for 30 sec.." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}

void Genetic::addRandomSolutions(int number) {
    //analysis..
    for(int i=0; i<number; i++){
        auto sol = std::make_shared<Solution>(generateRandomArray());
        processManagers_.push_back(std::make_shared<AnalysisManager>(sol, *this));
        population_.push_back(sol);
    }
    runPool();
}

std::array<bool, 20> Genetic::generateRandomArray() {
    std::uniform_real_distribution<double> dist;

    std::array<bool, 20> array{};
    double probability = numColumns_ / 20.0;

    for(int i=0; i<20; i++){
        array[i] = dist(randomGen_) <= probability;
    }

    return array;
}

void Genetic::sortPopulation() {
    std::sort(population_.begin(), population_.end(),
              [](std::shared_ptr<Solution> a,std::shared_ptr<Solution> b)
                {return a->getObjectiveFunction() < b->getObjectiveFunction();});
}

void Genetic::runPool(){
    //TODO vedi se va bene.
    int cpus = cpus_ - 2 <= 0 ? 1 : cpus_ - 2;
    int nthreads = std::min(cpus, static_cast<int>(processManagers_.size()));

    ctpl::thread_pool pool(nthreads);

    for(auto &man : processManagers_){
        pool.push(std::ref(*man));
    }

    //std::cout << "Running threads.." << std::endl;
    pool.stop(true);
    //std::cout << "Finished" << std::endl;
    processManagers_.clear();
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

/*
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
*/

void Genetic::initializeGenerator(){
    randomGen_ = std::mt19937(static_cast<unsigned int>(time(nullptr)));
}

void Genetic::checkAndSetBestSolution(std::shared_ptr<Solution> sol) {
    std::lock_guard<std::mutex> guard(mutex_);

    if(sol->getObjectiveFunction() < bestSolution_->getObjectiveFunction()) {
        bestSolution_ = std::make_shared<Solution>(sol->getObjectiveFunction(),sol->getArray());
        std::cout << "New best solution!" << std::endl;
        std::cout << bestSolution_->to_string() << std::endl;
    }
}

std::shared_ptr<Solution> Genetic::getBestSolution() {
    std::lock_guard<std::mutex> guard(mutex_);

    return bestSolution_;
}