#include <iostream>
#include <signal.h>
#include <memory>

#include "Genetic.h"

std::shared_ptr<Genetic> gen;
char programName[50] = "opensees 5MainFRPGen.tcl";

void signalHandler(int signal){
    std::cout << "Cleaning.." << std::endl;
    gen.reset();
    exit(0);
}

int main(int argc, char **argv) {
    if(argc < 5 || argc > 6){
        std::cout << "Usage: " << argv[0] << " [num_population] [num_elite] [pressure] [input_file] <output_file> " << std::endl;
        return 0;
    }

    //TODO vedi bene sti parametri
    const int NUM_POPULATION = atoi(argv[1]);
    const int NUM_ELITE = atoi(argv[2]);
    const int PRESSURE = atoi(argv[3]);

    //setting signal handler
    signal(SIGINT, signalHandler);

    if(argc == 5) {
        gen = std::make_shared<Genetic>(NUM_POPULATION, NUM_ELITE, PRESSURE, argv[4]);
    }
    else {
        gen = std::make_shared<Genetic>(NUM_POPULATION, NUM_ELITE, PRESSURE, argv[4], argv[5]);
    }

    //run
    gen->init();
    gen->run();

    gen.reset();

    //std::cout << "[debug] Exiting.." << std::endl;
}