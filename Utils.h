//
// Created by Gianluca on 27/05/2019.
//

#ifndef DENDEGENETIC_UTILS_H
#define DENDEGENETIC_UTILS_H

#include <vector>
#include <algorithm>
#include <exception>

namespace utils {
    inline std::vector<int> genIndexedVector(int size){
        std::vector<int> indexes{};
        indexes.reserve(size);

        for(int i=0; i<size; i++) indexes.push_back(i);

        return indexes;
    }

    class GeneticException : public std::exception {};
}

#endif //DENDEGENETIC_UTILS_H
