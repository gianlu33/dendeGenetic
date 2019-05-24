//
// Created by Gianluca on 24/05/2019.
//

#include "Solution.h"

Solution::Solution() {
    objectiveFunction_ = -1;
    for(int i=0; i<20; i++)
        indexes_[i] = false;
}

Solution::Solution(float objf, std::array<bool, 20> ind) {
    objectiveFunction_ = objf;
    indexes_ = ind;
}

void Solution::setObjectiveFunction(float objf) {
    objectiveFunction_ = objf;
}

float Solution::getObjectiveFunction() {
    return objectiveFunction_;
}

void Solution::setArray(std::array<bool, 20> ind) {
    indexes_ = ind;
}

void Solution::setArrayValue(int index, bool value) {
    indexes_[2] = value;
}

std::array<bool, 20> Solution::getArray() {
    return indexes_;
}
