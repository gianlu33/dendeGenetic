#include <iostream>
#include <signal.h>
#include <memory>

#include "Genetic.h"

//TODO da rimuovere
#include "AnalysisManager.h"
#include "Solution.h"
#include <array>
#include <windows.h>

std::shared_ptr<Genetic> gen;
char programName[50] = "opensees 5MainFRPGen.tcl";

void signalHandler(int signal){
    std::cout << "Cleaning.." << std::endl;
    gen.reset();
    exit(0);
}

int main(int argc, char **argv) {
    if(argc < 2 || argc > 3){
        std::cout << "Usage: " << argv[0] << " [input_file] <output_file>" << std::endl;
        return 0;
    }

    //TODO vedi bene sti parametri
    const int NUM_POPULATION = 8;
    const int NUM_ELITE = 1;

    //setting signal handler
    signal(SIGINT, signalHandler);

    if(argc == 2) {
        gen = std::make_shared<Genetic>(NUM_POPULATION, NUM_ELITE, argv[1]);
    }
    else {
        gen = std::make_shared<Genetic>(NUM_POPULATION, NUM_ELITE, argv[1], argv[2]);
    }

    //run
    gen->init();
    gen->run();

    gen.reset();

    //std::cout << "[debug] Exiting.." << std::endl;
}