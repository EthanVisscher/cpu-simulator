#include "cpu.h"

// Cpu class constructor
Cpu::Cpu()
{
    reset();
}

// parse client interface functions

// parse commands from file stream
void Cpu::parse(ifstream& infile, string command) 
{
    if (command == "set") {
        infile >> command;

        if (command == "reg") {
            uint8_t reg = RA;
            infile >> command;
            if (command == "PC")
                reg = PC;
            else
                reg = (command[1] - 'A');

            infile >> command;
            uint8_t val = stoi(command, 0, 16);
            setReg(reg, val);
        }
    }
}

// resets the program counter and all registers to 0
void Cpu::reset()
{
    for (int i = 0; i < CPU_NUM_OF_REGS; i++) {
        regs[i] = 0;
    }
}

// dumps the contents of the program counter and all registers
void Cpu::dump(ifstream& infile)
{
    printf("PC: 0x%X\n", pc);
    printf("RA: 0x%X\n", regs[CPU_REG_RA]);
    printf("RB: 0x%X\n", regs[CPU_REG_RB]);
    printf("RC: 0x%X\n", regs[CPU_REG_RC]);
    printf("RD: 0x%X\n", regs[CPU_REG_RD]);
    printf("RE: 0x%X\n", regs[CPU_REG_RE]);
    printf("RF: 0x%X\n", regs[CPU_REG_RF]);
    printf("RG: 0x%X\n", regs[CPU_REG_RG]);
    printf("RH: 0x%X\n", regs[CPU_REG_RH]);
    printf("\n");
}

// clock client interface functions

void Cpu::startTick() 
{

}

// called by clock to give cpu a chance to do some work
void Cpu::doCycleWork() 
{
    // shift values in registers
    for (int i = RH; i > RA; i--) 
        setReg(i, regs[i - 1]);

    // fetch instruction
    uint8_t fetchDone;
    memory->memStartFetch(pc, 1, &regs[RA], &fetchDone);

    pc++;
}

void Cpu::isMoreWorkNeeded() 
{

}

// cpu specific functions

// register memory device to cpu device
void Cpu::registerMemory(Memory* newMemory) 
{
    memory = newMemory;
}

// register instruction memory device to cpu device
void Cpu::registerMemory(IMemory* newIMemory) 
{
    imemory = newIMemory;
}

// sets the register <reg> to the value <val>
void Cpu::setReg(uint8_t reg, uint8_t val)
{
    if (reg == PC) {
        pc = val;
    }
    else {
        regs[reg] = val;
    }
}