//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_SOLUTION_H
#define DENDEGENETIC_SOLUTION_H

#include <array>

class Solution {
public:
    Solution();
    Solution(double objf, std::array<bool, 20> ind, bool feasible);
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
    bool isFeasible();
    bool setFeasible(bool feasible);
    static bool equals(Solution s1, Solution s2);
    static bool compare(Solution s1, Solution s2);
    double getFitness();
    void setFitness();
    int count1Columns();
    double computeFitness(double objf);

private:
    double objectiveFunction_;
    std::array<bool, 20> indexes_;
    static constexpr const double MAX_VALUE = 1e10;
    bool isFeasible_;
    double fitness_;
};

#endif //DENDEGENETIC_SOLUTION_H
