#include "cpu.h"

// Cpu class constructor
Cpu::Cpu()
{
    reset();
}

// parse commands from file stream
void Cpu::parse(ifstream& infile) 
{
    string command;
    infile >> command;

    if (command == "reset") {
        reset();
    } 
    else if (command == "set") {
        infile >> command;

        if (command == "reg") {
            CpuReg reg = RA;
            infile >> command;
            if (command == "PC") {
                reg = PC;
            }
            else {
                reg = (CpuReg)(command[1] - 'A');
            }

            infile >> command;
            uint8_t val = stoi(command, 0, 16);
            setReg(reg, val);
        }
    }
    else if (command == "dump") {
        dump();
    }
}

void Cpu::startTick() 
{

}

// called by clock to give cpu a chance to do some work
void Cpu::doCycleWork() 
{
    // fetch instruction
    uint8_t fetchByte;
    uint8_t fetchDone;
    memory->memStartFetch(pc, 1, &fetchByte, &fetchDone);
    pc += 1;

    // shift values in registers
    for (int i = 8; i > 1; i--) {
        setReg((CpuReg)i, regs[i - 1]);
    }
    regs[1] = fetchByte;
}

void Cpu::isMoreWorkNeeded() 
{

}

// register memory device to cpu device
void Cpu::registerMemory(Memory* newMemory) 
{
    memory = newMemory;
}

// resets the program counter and all registers to 0
void Cpu::reset()
{
    for (int i = 0; i < NUM_OF_REGS; i++) {
        regs[i] = 0;
    }
}

// sets the register <reg> to the value <val>
void Cpu::setReg(CpuReg reg, uint8_t val)
{
    if (reg == PC) {
        pc = val;
    }
    else {
        regs[reg] = val;
    }
}

// dumps the contents of the program counter and all registers
void Cpu::dump()
{
    printf("PC: 0x%X\n", pc);
    printf("RA: 0x%X\n", regs[RA]);
    printf("RB: 0x%X\n", regs[RB]);
    printf("RC: 0x%X\n", regs[RC]);
    printf("RD: 0x%X\n", regs[RD]);
    printf("RE: 0x%X\n", regs[RE]);
    printf("RF: 0x%X\n", regs[RF]);
    printf("RG: 0x%X\n", regs[RG]);
    printf("RH: 0x%X\n", regs[RH]);
    printf("\n");
}

