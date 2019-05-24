#include <iostream>
#include <thread>

int main(int argc, char **argv) {
    if(argc < 2 || argc > 3){
        std::cout << "Usage: " << argv[0] << " [input_file] <output_file>" << std::endl;
    }

    return 0;
}