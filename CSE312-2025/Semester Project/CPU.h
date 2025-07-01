#ifndef CPU_H
#define CPU_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

// Define instruction types
enum class InstructionType {
    SET, CPY, CPYI, CPYI2, ADD, ADDI, SUBI, JIF, PUSH, POP, CALL, RET, HLT, USER, SYSCALL, EMPTY
};

// Define system call types
enum class SyscallType {
    PRN, HLT, YIELD
};

// Memory location constants
const int PC_REG = 0;        // Program Counter
const int SP_REG = 1;        // Stack Pointer
const int SYSCALL_RESULT = 2; // System call result
const int INSTR_COUNT = 3;    // Number of instructions executed
const int KERNEL_BOUNDARY = 1000; // Boundary between kernel and user space
const int MAX_THREADS = 11; // Maximum number of threads 10 threads + 1 OS thread

class Instruction {
public:
    InstructionType type;
    SyscallType syscallType;
    int param1;
    int param2;
    std::string rawInstruction;

    Instruction() : type(InstructionType::EMPTY), syscallType(SyscallType::HLT), param1(0), param2(0) {}
};

class CPU {
    private:
        std::vector<long> DataMemory;
        std::vector<Instruction> InstructionMemory;
        bool kernelMode;
        bool halted;
        long int prevThread;
        long int currentThread;
        int debugLevel;
        
        // Parse instruction from string
        Instruction parseInstruction(const std::string& line, int threadNum);
        
        // Execute one instruction
        void executeInstruction(const Instruction& instr);
        void adjustBlockedTable();
        
        // Helper methods
        void switchToKernelMode();
        void switchToUserMode();
        void dumpMemory();
        
    public:
        CPU(int debugLevel = 0);
        ~CPU(); // Destructor to clean up dynamically allocated memory
        
        // Load program from file
        bool loadProgram(const std::string& filename);
        
        // Execute the next instruction
        void execute();
        
        // Check if CPU is halted
        bool isHalted() const;
        
        // Run the simulation
        void run();
};

#endif
