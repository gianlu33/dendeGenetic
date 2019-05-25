//
// Created by Gianluca on 24/05/2019.
//

#ifndef DENDEGENETIC_LOCALSEARCHMANAGER_H
#define DENDEGENETIC_LOCALSEARCHMANAGER_H

#include "ProcessManager.h"

class LocalSearchManager : public ProcessManager {
public:
    explicit LocalSearchManager(std::shared_ptr<Solution> sol);
    ~LocalSearchManager();
    void operator()(int id) override;
};


#endif //DENDEGENETIC_LOCALSEARCHMANAGER_H
