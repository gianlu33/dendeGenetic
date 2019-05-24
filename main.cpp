#include <iostream>
#include <thread>
#include <signal.h>
#include <vector>

#include "Genetic.h"
#include "Solution.h"

char programName[20] = "prova.exe";
std::shared_ptr<Genetic> gen;


void signalHandler(int signal){
    gen.reset();
    exit(0);
}

int main(int argc, char **argv) {
    /*
    if(argc < 2 || argc > 3){
        std::cout << "Usage: " << argv[0] << " [input_file] <output_file>" << std::endl;
    }*/

    char tmp[10] = "ciao";
    gen = std::make_shared<Genetic>(16, tmp);

    //setting signal handler
    signal(SIGINT, signalHandler);

    //run
    gen->run();

    gen.reset();
    return 0;
}