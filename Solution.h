//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_SOLUTION_H
#define DENDEGENETIC_SOLUTION_H

#include <array>

class Solution {
public:
    Solution();
    Solution(double objf, std::array<bool, 20> ind);
    Solution(std::array<bool, 20> ind);
    void setObjectiveFunction(double objf);
    double getObjectiveFunction();
    void setArray(std::array<bool, 20> ind);
    void setArrayValue(int index, bool value);
    bool getArrayValue(int index);
    void flip(int index);
    std::array<bool, 20> getArray(); //copied
    std::string to_string();
    std::string getArrayString();

private:
    double objectiveFunction_;
    std::array<bool, 20> indexes_;
    static constexpr const double MAX_VALUE = 1e10;
};

#endif //DENDEGENETIC_SOLUTION_H
