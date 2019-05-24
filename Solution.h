//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_SOLUTION_H
#define DENDEGENETIC_SOLUTION_H

#include <array>

class Solution {
public:
    Solution();
    Solution(float objf, std::array<bool, 20> ind);
    void setObjectiveFunction(float objf);
    float getObjectiveFunction();
    void setArray(std::array<bool, 20> ind);
    void setArrayValue(int index, bool value);
    std::array<bool, 20> getArray();

private:
    float objectiveFunction_;
    std::array<bool, 20> indexes_;
};

#endif //DENDEGENETIC_SOLUTION_H
