//
// Created by Gianluca on 24/05/2019.
//

#include "IOManager.h"
#include <fstream>
#include <array>
#include <cmath>

#include <iostream>

std::vector<std::shared_ptr<Solution>> IOManager::readInput(char *filename) {
    std::vector<std::shared_ptr<Solution>> sols;
    std::ifstream ifs(filename);
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    int n;

    //read n rows
    ifs >> n;

    for(int i=0; i<n; i++){
        float objf;
        bool feasible;

        std::array<bool, 20> array{};

        //read objf
        ifs >> objf;

        //read feasible
        ifs >> feasible;

        //read element of a solution
        for(int j=0; j<20; j++)
            ifs >> array[j];

        sols.push_back(std::make_shared<Solution>(objf, array, feasible));
    }

    ifs.close();

    return sols;
}

void IOManager::writeOutput(char *filename, std::vector<std::shared_ptr<Solution>> &vect) {
    std::ofstream ofs(filename);
    ofs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    ofs << vect.size() << std::endl;

    for(auto& sol : vect){
        ofs << sol->to_string();
        ofs << std::endl;
    }
}

double IOManager::getMaxAbsValue(std::string filename) {
    std::ifstream ifs(filename);
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    double max = -1, temp;

    while(true){
        try {
            ifs >> temp; //nothing
        }
        catch(...){
            //end of file
            break;
        }

        ifs >> temp; //value

        temp = std::abs(temp);

        if(max == -1 || temp > max)
            max = temp;
    }

    return max;
}
