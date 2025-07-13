
#include "CPU.h"
#include <iostream>
#include <string>

void printUsage() {
    std::cout << "Usage: Simulate <filename> -D <debug_level>" << std::endl;
    std::cout << "Debug levels:" << std::endl;
    std::cout << "  0 - Run program normally, dump memory at the end." << std::endl;
    std::cout << "  1 - Dump memory after each instruction." << std::endl;
    std::cout << "  2 - Dump memory after each instruction and wait for keypress." << std::endl;
    std::cout << "  3 - Print context table after each context switch." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }
    
    std::string filename = argv[1];
    int debugLevel = 0;
    
    // Parse command line arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-D" && i + 1 < argc) {
            debugLevel = std::stoi(argv[++i]);
        }
    }
    
    // Create CPU with specified debug level
    CPU cpu(debugLevel);
    
    // Load program
    if (!cpu.loadProgram(filename)) {
        std::cerr << "Failed to load program from file: " << filename << std::endl;
        return 1;
    }
    
    std::cout << "Starting simulation with debug level " << debugLevel << std::endl;
    
    // Run the simulation
    cpu.run();
    
    std::cout << "Simulation complete!" << std::endl;
    
    return 0;
}