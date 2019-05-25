//
// Created by Gianluca on 24/05/2019.
//

#include "LocalSearchManager.h"
#include <cstring>
#include <string>
#include <iostream>

LocalSearchManager::LocalSearchManager(std::shared_ptr<Solution> sol) : ProcessManager(std::move(sol)) {

}

LocalSearchManager::~LocalSearchManager() {
    //std::cout << "destroying LocalSearchManager" << std::endl;
}

void LocalSearchManager::operator()(int id) {

}

