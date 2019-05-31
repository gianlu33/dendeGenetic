//
// Created by Gianluca on 24/05/2019.
//

#include "Solution.h"

#include <sstream>

Solution::Solution() {
    objectiveFunction_ = MAX_VALUE;
    for(int i=0; i<20; i++)
        indexes_[i] = false;
}

Solution::Solution(double objf, std::array<bool, 20> ind) {
    objectiveFunction_ = objf;
    indexes_ = ind;
}

Solution::Solution(std::array<bool, 20> ind) {
    objectiveFunction_ = MAX_VALUE;
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

    ss << objectiveFunction_ << std::endl << getArrayString();

    return ss.str();
}

std::string Solution::getArrayString() {
    std::stringstream ss;

    for(auto &value : indexes_)
        ss << value << " ";

    return ss.str();
}