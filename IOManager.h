//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_IOMANAGER_H
#define DENDEGENETIC_IOMANAGER_H

#include <vector>
#include <memory>

#include "Solution.h"

class Solution;

class IOManager {
public:
    static std::vector<std::shared_ptr<Solution>> readInput(char *filename);
    static void writeOutput(char *filename, std::vector<std::shared_ptr<Solution>> &vect);
    //TODO function to read a solution from the analysis


};


#endif //DENDEGENETIC_IOMANAGER_H
