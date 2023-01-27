#include "cpu.h"

// Cpu class constructor
Cpu::Cpu()
{
    reset();
}

// called by clock to give cpu a chance to do some work
void Cpu::doCycleWork() 
{
    // fetch instruction
    uint8_t fetchByte;
    uint8_t fetchDone;
    memory->memStartFetch(regs[PC], 1, &fetchByte, &fetchDone);
    regs[PC] += 1;

    // shift values in registers
    for (int i = 8; i > 1; i--) {
        setReg(i, regs[i - 1]);
    }
    regs[1] = fetchByte;
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
        string setCommand;
        infile >> setCommand;
        if (setCommand == "reg") {
            infile >> command;

            uint8_t reg;
            if (command == "PC")
                reg = PC;
            if (command == "RA")
                reg = RA;
            if (command == "RB")
                reg = RB;
            if (command == "RC")
                reg = RC;
            if (command == "RD")
                reg = RD;
            if (command == "RE")
                reg = RE;
            if (command == "RF")
                reg = RF;
            if (command == "RG")
                reg = RG;
            if (command == "RH")
                reg = RH;

            infile >> command;
            uint8_t val = stoi(command, 0, 16);

            setReg(reg, val);
        }
    }
    else if (command == "dump") {
        dump();
    }
}

// register memory device to cpu device
void Cpu::registerMemory(Memory* newMemory) 
{
    memory = newMemory;
}

// resets the program counter and all registers to 0
void Cpu::reset()
{
    for (int i = 0; i < 8; i++) {
        regs[i] = 0;
    }
}

// sets the register <reg> to the value <val>
void Cpu::setReg(uint8_t reg, uint8_t val)
{
    // out of bounds
    if (reg < PC  || reg > RH) {
        return;
    }

    regs[reg] = val;
}

// dumps the contents of the program counter and all registers
void Cpu::dump()
{
    printf("PC: 0x%X\n", regs[PC]);
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

