#include "CPU.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iomanip>

// Helper function to trim whitespace
std::string trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](int c) { return std::isspace(c); });
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](int c) { return std::isspace(c); }).base();
    return (start < end) ? std::string(start, end) : std::string();
}

bool CPU::loadProgram(const std::string& filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    bool inDataSection = false;
    bool inInstructionSection = false;
    int InstMemoryAddress = 0;
    int threadNum = 0;

    while (std::getline(file, line)) {
        
        // Remove comments
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        
        // Trim whitespace
        line = trim(line);
        
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Check for section markers
        if (line == "Begin Data Section") {
            inDataSection = true;
            inInstructionSection = false;
            continue;
        }
        else if (line == "End Data Section") {
            inDataSection = false;
            continue;
        }
        else if (line == "Begin Instruction Section") {
            inInstructionSection = true;
            inDataSection = false;
            //dynamic_cast<DataValue*>(memory[SP_REG + threadNum*1000])->value = 1000*(threadNum+1);
            continue;
        }
        else if (line == "End Instruction Section") {
            inInstructionSection = false;
            threadNum++;
            InstMemoryAddress = 0;
            continue;
        }
        
        // Process data or instructions
        if (inDataSection) {
            std::istringstream iss(line);
            int address, value;
            if (iss >> address >> value) {
                DataMemory[address + threadNum*1000] = value;
            }
        }
        else if (inInstructionSection) {
            Instruction instr = parseInstruction(line, threadNum);
            InstructionMemory[InstMemoryAddress + threadNum*1000] = instr;
            InstMemoryAddress++;
        }
    }
    
    file.close();
    return true;
}

void CPU::dumpMemory(){
    for(int j = 0; j < MAX_THREADS; j++){
        for(int i = 0; i < 1000; i++){
            if(DataMemory[i + j*1000] != 0){
                std::cerr << "Data MemoryAddress " << i + j*1000 << ": " << DataMemory[i + j*1000] << std::endl;
            }
        }
    }
    for(int j = 0; j < MAX_THREADS; j++){
        for(int i = 0; i < 1000; i++){
            if(InstructionMemory[i + j*1000].type != InstructionType::EMPTY){
                std::cerr << "Instruction MemoryAddress " << i + j*1000 << ": " << InstructionMemory[i + j*1000].rawInstruction << " " << InstructionMemory[i + j*1000].param1 << " " << InstructionMemory[i + j*1000].param2 << std::endl;
            }
        }
    }
}

Instruction CPU::parseInstruction(const std::string& line, int threadNum) 
{
    Instruction instr;
    std::string instructionStr;
    std::istringstream iss(line);
    
    // Skip instruction number if present
    std::string firstToken;
    iss >> firstToken;
    
    // Check if firstToken is a number (instruction address)
    bool isInstructionNum = true;
    for (char c : firstToken) {
        if (!std::isdigit(c)) {
            isInstructionNum = false;
            break;
        }
    }
    
    if (isInstructionNum) {
        iss >> instructionStr;
    } else {
        instructionStr = firstToken;
    }
    
    instr.rawInstruction = line;
    
    // Parse instruction type
    if (instructionStr == "SET") {
        instr.type = InstructionType::SET;
        iss >> instr.param1 >> instr.param2;
        if(instr.param2 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 > 20){
            instr.param2 += threadNum*1000;
        }
        if(instr.param2 == 0){
            instr.param1 += threadNum*1000;
        }
    } 
    else if (instructionStr == "CPY") {
        instr.type = InstructionType::CPY;
        iss >> instr.param1 >> instr.param2;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 > 20){
            instr.param2 += threadNum*1000;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "CPYI") {
        instr.type = InstructionType::CPYI;
        iss >> instr.param1 >> instr.param2;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 > 20){
            instr.param2 += threadNum*1000;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }else if(instructionStr == "CPYI2"){
        instr.type = InstructionType::CPYI2;
        iss >> instr.param1 >> instr.param2;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 > 20){
            instr.param2 += threadNum*1000;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "ADD") {
        instr.type = InstructionType::ADD;
        iss >> instr.param1 >> instr.param2;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "ADDI") {
        instr.type = InstructionType::ADDI;
        iss >> instr.param1 >> instr.param2;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 > 20){
            instr.param2 += threadNum*1000;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "SUBI") {
        instr.type = InstructionType::SUBI;
        iss >> instr.param1 >> instr.param2;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 > 20){
            instr.param2 += threadNum*1000;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "JIF") {
        instr.type = InstructionType::JIF;
        iss >> instr.param1 >> instr.param2;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param2 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        instr.param2 += threadNum*1000;
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "PUSH") {
        instr.type = InstructionType::PUSH;
        iss >> instr.param1;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "POP") {
        instr.type = InstructionType::POP;
        iss >> instr.param1;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "CALL") {
        instr.type = InstructionType::CALL;
        iss >> instr.param1;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "RET") {
        instr.type = InstructionType::RET;
    }
    else if (instructionStr == "HLT") {
        instr.type = InstructionType::HLT;
    }
    else if (instructionStr == "USER") {
        instr.type = InstructionType::USER;
        iss >> instr.param1;
        if(instr.param1 < 0){
            std::cerr << "Error: Memory cannot be negative" << std::endl;
            halted = true;
            return instr;
        }
        if(instr.param1 > 20){
            instr.param1 += threadNum*1000;
        }
    }
    else if (instructionStr == "SYSCALL") {
        instr.type = InstructionType::SYSCALL;
        std::string syscallType;
        iss >> syscallType;
        
        if (syscallType == "PRN") {
            instr.syscallType = SyscallType::PRN;
            iss >> instr.param1;
            if(instr.param1 < 0){
                std::cerr << "Error: Memory cannot be negative" << std::endl;
                halted = true;
                return instr;
            }
            if(instr.param1 > 20){
                instr.param1 += threadNum*1000;
            }
        }
        else if (syscallType == "HLT") {
            instr.syscallType = SyscallType::HLT;
        }
        else if (syscallType == "YIELD") {
            instr.syscallType = SyscallType::YIELD;
        }
    }
    
    return instr;
}

void CPU::execute() {
    // Get current PC
    long int pcAddress = DataMemory[PC_REG];
    prevThread = currentThread;
    currentThread = DataMemory[29];
    // Check if PC is within bounds
    if (pcAddress < 0 || pcAddress >= static_cast<long int>(DataMemory.size())) {
        std::cerr << "Error: PC out of bounds: " << pcAddress << std::endl;
        halted = true;
        return;
    }
    
    // Check if memory[pc] is null
    if (InstructionMemory[pcAddress].type == InstructionType::EMPTY) {
        std::cerr << "Error: Null instruction at PC: " << pcAddress << std::endl;
        halted = true;
        return;
    }
    
    // Get instruction directly from PC location
    Instruction instr = InstructionMemory[pcAddress];
    if (instr.type == InstructionType::EMPTY) {
        std::cerr << "Error: Expected instruction at PC: " << pcAddress << std::endl;
        halted = true;
        return;
    }
    
    //std::cout << "PC: " << pcAddress << std::endl;
    
    // Increment instruction count 
    DataMemory[INSTR_COUNT]++;
    adjustBlockedTable();
    
    //std::cout << "Executing instruction: " << instr.rawInstruction << " " << instr.param1 << " " << instr.param2 << " PC: " << pcAddress << std::endl;
    // Execute instruction
    executeInstruction(instr);
    
    if (debugLevel == 1) {
        dumpMemory();
    }
    else if (debugLevel == 2) {
        dumpMemory();
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.get();  // Wait for keypress
    } else if (debugLevel == 3) { 
        if (prevThread != currentThread) {
            int threadId = DataMemory[currentThread - 24];
            int startTime = DataMemory[currentThread - 23];
            int instrCount = DataMemory[currentThread - 22];
            int state = DataMemory[currentThread - 21];

            std::cerr << "\n==========================================" << std::endl;
            std::cerr << "         Thread " << threadId << " Context Table" << std::endl;
            std::cerr << "==========================================" << std::endl;
            std::cerr << "Thread ID         : " << std::setw(22) << threadId << std::endl;
            std::cerr << "Starting Time     : " << std::setw(22) << startTime << std::endl;
            std::cerr << "Instruction Count : " << std::setw(22) << instrCount << std::endl;

            std::cerr << "State             : ";
            switch (state) {
                case 0: std::cerr << std::setw(22) << "0 (Halted)"; break;
                case 1: std::cerr << std::setw(22) << "1 (Ready)"; break;
                case 2: std::cerr << std::setw(22) << "2 (Running)"; break;
                case 3: std::cerr << std::setw(22) << "3 (Blocked)"; break;
                default: std::cerr << state << " (Unknown)"; break;
            }
            std::cerr << std::endl;

            std::cerr << "------------------------------------------" << std::endl;
            for (int i = (currentThread - 20), j = 0; i <= currentThread; i++, j++) {
                std::cerr << "Register[" << std::setw(2) << j << "]      : " << std::setw(22) << DataMemory[i] << std::endl;
            }
            std::cerr << "==========================================\n" << std::endl;
        }
    }
}

void CPU::executeInstruction(const Instruction& instr) {
    // Increment PC by default (may be overridden by jumps)
    DataMemory[PC_REG]++;
    
    int threadNum = (DataMemory[21] - 54)/25;

    switch (instr.type) {
        case InstructionType::SET: {
            if (!kernelMode && ((instr.param2 < KERNEL_BOUNDARY && !(instr.param2 <= 20)) || instr.param2 > (threadNum + 1)*1000)) {
                std::cerr << "Error: Memory access violation in SET" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[instr.param2] = instr.param1;
            break;
        }
        
        case InstructionType::CPY: {
            if (!kernelMode && 
                (( (instr.param1 < KERNEL_BOUNDARY && !(instr.param1 <= 20)) || instr.param1 > (threadNum + 1)*1000 ) || ((instr.param2 < KERNEL_BOUNDARY && !(instr.param2 <= 20)) || instr.param2 > (threadNum + 1)*1000))) {  
                std::cerr << "Error: Memory access violation in CPY" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[instr.param2] = DataMemory[instr.param1];
            break;
        }
        
        case InstructionType::CPYI: {
            if (!kernelMode && 
                (( (instr.param1 < KERNEL_BOUNDARY && !(instr.param1 <= 20)) || instr.param1 > (threadNum + 1)*1000 ) || ((instr.param2 < KERNEL_BOUNDARY && !(instr.param2 <= 20)) || instr.param2 > (threadNum + 1)*1000)  
                || ((DataMemory[instr.param1] < KERNEL_BOUNDARY && !(DataMemory[instr.param1] <= 20)) || DataMemory[instr.param1] > (threadNum + 1)*1000))) {
                std::cerr << "CPYI: " << instr.param1 << " " << instr.param2 << std::endl;
                std::cerr << "Thread number: " << threadNum << std::endl;
                std::cerr << "Error: Memory access violation in CPYI" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[instr.param2] = DataMemory[DataMemory[instr.param1]];
            break;
        }

        case InstructionType::CPYI2: {
            if (!kernelMode && 
                (( (instr.param1 < KERNEL_BOUNDARY && !(instr.param1 <= 20)) || instr.param1 > (threadNum + 1)*1000 ) || ((instr.param2 < KERNEL_BOUNDARY && !(instr.param2 <= 20)) || instr.param2 > (threadNum + 1)*1000) || 
                ((DataMemory[instr.param1] < KERNEL_BOUNDARY && !(DataMemory[instr.param1] <= 20)) || DataMemory[instr.param1] > (threadNum + 1)*1000))) {
                std::cerr << "Error: Memory access violation in CPYI2" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[DataMemory[instr.param2]] = DataMemory[DataMemory[instr.param1]];
            break;
        }
        
        case InstructionType::ADD: {
            if (!kernelMode && ((instr.param1 < KERNEL_BOUNDARY && !(instr.param1 <= 20)) || instr.param1 > (threadNum + 1)*1000)) {
                std::cerr << "Error: Memory access violation in ADD" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[instr.param1] += instr.param2;
            break;
        }
        
        case InstructionType::ADDI: {
            if (!kernelMode && 
                (( (instr.param1 < KERNEL_BOUNDARY && !(instr.param1 <= 20)) || instr.param1 > (threadNum + 1)*1000 ) || ((instr.param2 < KERNEL_BOUNDARY && !(instr.param2 <= 20)) || instr.param2 > (threadNum + 1)*1000))) {
                std::cerr << "Error: Memory access violation in ADDI" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[instr.param2] += DataMemory[instr.param1];
            break;
        }
        
        case InstructionType::SUBI: {
            if (!kernelMode && 
                (( (instr.param1 < KERNEL_BOUNDARY && !(instr.param1 <= 20)) || instr.param1 > (threadNum + 1)*1000 ) || ((instr.param2 < KERNEL_BOUNDARY && !(instr.param2 <= 20)) || instr.param2 > (threadNum + 1)*1000))) {
                std::cerr << "Error: Memory access violation in SUBI" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[instr.param2] = DataMemory[instr.param1] - DataMemory[instr.param2];
            break;
        }
        
        case InstructionType::JIF: {
            if (!kernelMode && ((instr.param1 < KERNEL_BOUNDARY && !(instr.param1 <= 20)) || instr.param1 > (threadNum + 1)*1000)) {
                std::cerr << "Error: Memory access violation in JIF" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            //std::cout << "JIF: " << DataMemory[PC_REG] << " " << DataMemory[instr.param1] << " " << instr.param2 << std::endl;
            if (DataMemory[instr.param1] <= 0) {
                DataMemory[PC_REG] = instr.param2;
            }
            break;
        }
        
        case InstructionType::PUSH: {
            if (!kernelMode && 
                (( (instr.param1 < KERNEL_BOUNDARY && !(instr.param1 <= 20)) || instr.param1 > (threadNum + 1)*1000 ) || ((DataMemory[SP_REG] < KERNEL_BOUNDARY && !(DataMemory[SP_REG] <= 20)) || DataMemory[SP_REG] > (threadNum + 1)*1000))) {
                std::cerr << "Error: Memory access violation in PUSH" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[SP_REG]--;
            if(DataMemory[SP_REG] < 0){
                std::cout << "Stack overflows" << std::endl;
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
                return;
            } else {
                DataMemory[DataMemory[SP_REG]] = DataMemory[instr.param1];
            }
            break;
        }
        
        case InstructionType::POP: {
            if (!kernelMode && 
                (( (instr.param1 < KERNEL_BOUNDARY && !(instr.param1 <= 20)) || instr.param1 > (threadNum + 1)*1000 ) || ((DataMemory[SP_REG] + 1 < KERNEL_BOUNDARY && !(DataMemory[SP_REG] + 1 <= 20)) || DataMemory[SP_REG] + 1 > (threadNum + 1)*1000))) {
                std::cerr << "Error: Memory access violation in POP" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[instr.param1] = DataMemory[DataMemory[SP_REG]];
            DataMemory[SP_REG]++;
            break;
        }
        
        case InstructionType::CALL: {
            if (!kernelMode && ((DataMemory[SP_REG] < KERNEL_BOUNDARY && !(DataMemory[SP_REG] <= 20)) || DataMemory[SP_REG] > (threadNum + 1)*1000)) {
                std::cerr << "Error: Memory access violation in CALL" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            //Push the return address to the stack
            DataMemory[SP_REG]--;
            if(DataMemory[SP_REG] < 0){
                std::cout << "Stack overflows" << std::endl;
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
                return;
            } else {
                DataMemory[DataMemory[SP_REG]] = DataMemory[PC_REG];
            }
            //Jump to the address in the param1 register
            DataMemory[PC_REG] = instr.param1;
            break;
        }
        
        case InstructionType::RET: {
            if (!kernelMode && ((DataMemory[SP_REG] + 1 < KERNEL_BOUNDARY && !(DataMemory[SP_REG] + 1 <= 20)) || DataMemory[SP_REG] + 1 > (threadNum + 1)*1000)) {
                std::cerr << "Error: Memory access violation in RET" << std::endl;
                //Halt that thread
                switchToKernelMode();
                DataMemory[500] = DataMemory[PC_REG];
                DataMemory[PC_REG] = 80;
            }
            DataMemory[PC_REG] = DataMemory[DataMemory[SP_REG]];
            DataMemory[SP_REG]++;
            break;
        }
        
        case InstructionType::HLT: {
            halted = true;
            break;
        }
        
        case InstructionType::USER: {
            switchToUserMode();
            DataMemory[PC_REG] = DataMemory[instr.param1];
            break;
        }
        
        case InstructionType::SYSCALL: {
            // Switch to kernel mode for syscalls
            switchToKernelMode();
            
            switch (instr.syscallType) {
                case SyscallType::PRN: //This syscall is handled by cpp not by OS just like mentioned in the document
                    switchToKernelMode();
                    std::cout << "SYSCALL PRN: " << DataMemory[instr.param1] << " - Thread ID: " << DataMemory[currentThread - 24] << std::endl;
                    DataMemory[DataMemory[21] - 21] = 3;
                    DataMemory[(DataMemory[21] - 79)/25 + 601] = 100;
                    DataMemory[500] = DataMemory[PC_REG];
                    DataMemory[PC_REG] = 90;
                    break;
                    
                case SyscallType::HLT:
                    switchToKernelMode();
                    /*TODO:  Jump to a fixed syscall handler address in OS space. Let the OS handle the syscall*/
                    std::cout << "SYSCALL HLT " << "- Thread ID: " << DataMemory[currentThread - 24] << std::endl;
                    DataMemory[500] = DataMemory[PC_REG];
                    DataMemory[PC_REG] = 80;
                    break;
                    
                case SyscallType::YIELD:
                    switchToKernelMode();
                    /*TODO:  Jump to a fixed syscall handler address in OS space. Let the OS handle the syscall*/
                    std::cout << "SYSCALL YIELD " << "- Thread ID: " << DataMemory[currentThread - 24] << std::endl;
                    DataMemory[500] = DataMemory[PC_REG];
                    DataMemory[PC_REG] = 86;
                    //handleSyscallYield();
                    break;
            }
            break;
        }

        case InstructionType::EMPTY: {
            std::cerr << "Error: Empty instruction! Something went wrong!" << std::endl;
            //handleSyscallHlt();
            break;
        }
    }
}

void CPU::adjustBlockedTable() {
    for(int i=601; i<=610; i++){
        if(DataMemory[i] > 0){
            DataMemory[i]--;
            if(DataMemory[i] == 0){
                DataMemory[(i-600)*25+33] = 1;
            }
        }
    }
}

void CPU::switchToKernelMode() {
    kernelMode = true;
}

void CPU::switchToUserMode() {
    kernelMode = false;
}

bool CPU::isHalted() const {
    return halted;
}

CPU::CPU(int debugLevel) : kernelMode(true), halted(false), debugLevel(debugLevel) {
    // Initialize memory
    DataMemory.resize((MAX_THREADS + 1)*1000, 0);
    InstructionMemory.resize((MAX_THREADS + 1)*1000, Instruction());
    currentThread = DataMemory[29];
    prevThread = currentThread;
}

CPU::~CPU() {
    // Clean up all dynamically allocated memory
    DataMemory.clear();
    InstructionMemory.clear();
}

void CPU::run() {
    while (!isHalted()) {
        execute();
    }
    
    if (debugLevel == 0) {
        dumpMemory();
    }
}