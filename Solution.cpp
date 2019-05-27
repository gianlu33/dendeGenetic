//
// Created by Gianluca on 24/05/2019.
//

#include "Solution.h"

#include <sstream>
#include <limits>

Solution::Solution() {
    objectiveFunction_ = std::numeric_limits<double>::max();
    for(int i=0; i<20; i++)
        indexes_[i] = false;
}

Solution::Solution(double objf, std::array<bool, 20> ind) {
    objectiveFunction_ = objf;
    indexes_ = ind;
}

Solution::Solution(std::array<bool, 20> ind) {
    objectiveFunction_ = std::numeric_limits<double>::max();
    indexes_ = ind;
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

    ss << objectiveFunction_ << std::endl;

    for(auto &value : indexes_)
        ss << value << " ";

    return ss.str();
}