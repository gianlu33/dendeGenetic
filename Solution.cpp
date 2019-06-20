//
// Created by Gianluca on 24/05/2019.
//

#include "Solution.h"

#include <sstream>

Solution::Solution() {
    objectiveFunction_ = MAX_VALUE;
    fitness_ = MAX_VALUE;
    for(int i=0; i<20; i++)
        indexes_[i] = false;
    isFeasible_ = false;
}

Solution::Solution(double objf, std::array<bool, 20> ind, bool feasible) {
    objectiveFunction_ = objf;
    indexes_ = ind;
    isFeasible_ = feasible;
    setFitness();
}

Solution::Solution(std::array<bool, 20> ind) {
    objectiveFunction_ = MAX_VALUE;
    fitness_ = MAX_VALUE;
    indexes_ = ind;
    isFeasible_ = false;
}

void Solution::setObjectiveFunction(double objf) {
    objectiveFunction_ = objf;
}

double Solution::getObjectiveFunction() {
    return objectiveFunction_;
}

void Solution::setArray(std::array<bool, 20> ind) {
    indexes_ = ind;
}

void Solution::setArrayValue(int index, bool value) {
    indexes_[2] = value;
}

bool Solution::getArrayValue(int index) {
    return indexes_[index];
}

void Solution::flip(int index) {
    indexes_[index] = !indexes_[index];
}

std::array<bool, 20> Solution::getArray() {
    return indexes_;
}

std::string Solution::to_string() {
    std::stringstream ss;

    ss << objectiveFunction_  << " " << fitness_ << " " << isFeasible_ << std::endl << getArrayString();

    return ss.str();
}

std::string Solution::getArrayString() {
    std::stringstream ss;

    for(auto &value : indexes_)
        ss << value << " ";

    return ss.str();
}

bool Solution::isFeasible() {
    return isFeasible_;
}

bool Solution::setFeasible(bool feasible) {
    isFeasible_ = feasible;
}

bool Solution::equals(Solution s1, Solution s2) {
    return s1.objectiveFunction_ == s2.objectiveFunction_ && s1.isFeasible_ == s2.isFeasible_ && s1.indexes_ == s2.indexes_;
}

bool Solution::compare(Solution s1, Solution s2) {
    return s1.fitness_ < s2.fitness_;
}

double Solution::getFitness() {
    return fitness_;
}

void Solution::setFitness() {
    fitness_ = computeFitness(objectiveFunction_);
}

int Solution::count1Columns() {
    int n = 0;

    for(int i=0; i<20; i++){
        if(indexes_[i]) n++;
    }

    return n;
}

double Solution::computeFitness(double objf) {
    return objf;
}
