#include <iostream>
#include <signal.h>
#include <memory>

#include "Genetic.h"

char programName[20] = "prova.exe";
std::shared_ptr<Genetic> gen;


void signalHandler(int signal){
    gen.reset();
    exit(0);
}

int main(int argc, char **argv) {
    if(argc < 2 || argc > 3){
        std::cout << "Usage: " << argv[0] << " [input_file] <output_file>" << std::endl;
        return 0;
    }

    if(argc == 2) {
        gen = std::make_shared<Genetic>(16, 1, argv[1]);
    }
    else {
        gen = std::make_shared<Genetic>(16, 1, argv[1], argv[2]);
    }

    //setting signal handler
    signal(SIGINT, signalHandler);

    //run
    gen->run();

    gen.reset();

    return 0;
}