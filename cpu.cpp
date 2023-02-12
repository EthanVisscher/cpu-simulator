#include "cpu.h"

// Cpu class constructor
Cpu::Cpu()
{
    reset();
    state = CPU_STATE_IDLE;
}

// parse client interface functions

// parse commands from file stream
void Cpu::parse(ifstream& infile, string command) 
{
    if (command == "set") {
        infile >> command;

        if (command == "reg") {
            uint8_t reg = 0;
            infile >> command;
            if (command == "PC")
                reg = CPU_REG_PC;
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
    pc = 0x0;
    for (int i = 0; i < CPU_NUM_OF_REGS; i++) {
        regs[i] = 0;
    }
}

// dumps the contents of the program counter and all registers
void Cpu::dump(ifstream& infile)
{
    printf("PC: 0x%02X\n", pc);
    printf("RA: 0x%02X\n", regs[CPU_REG_RA]);
    printf("RB: 0x%02X\n", regs[CPU_REG_RB]);
    printf("RC: 0x%02X\n", regs[CPU_REG_RC]);
    printf("RD: 0x%02X\n", regs[CPU_REG_RD]);
    printf("RE: 0x%02X\n", regs[CPU_REG_RE]);
    printf("RF: 0x%02X\n", regs[CPU_REG_RF]);
    printf("RG: 0x%02X\n", regs[CPU_REG_RG]);
    printf("RH: 0x%02X\n", regs[CPU_REG_RH]);
    printf("\n");
}

// clock client interface functions

void Cpu::startTick() 
{
    working = 1;
    if (state == CPU_STATE_IDLE) {
        state = CPU_STATE_FETCH;
    } 
}

// called by clock to give cpu a chance to do some work
void Cpu::doCycleWork() 
{
    if (state == CPU_STATE_WAIT) {
        // check if fetch or store is done
        if (fsDone == 1) {
            if (instruction.inst == CPU_INST_LW)
                regs[instruction.dest] = fsData;
            state = CPU_STATE_IDLE;
        }

        working = 0;
    }

    if (working == 0)
        return;

    if (state == CPU_STATE_FETCH) {
        decodeInstruction(imemory->fetchInstruction(pc));
        pc++;
        state = CPU_STATE_EXEC;
    }
    else if (state == CPU_STATE_EXEC) {
        // load word
        if (instruction.inst == CPU_INST_LW) {
            memory->memStartFetch(regs[instruction.tarReg], 1, &fsData, &fsDone);
        }
        // store word
        else if (instruction.inst == CPU_INST_SW) {
            fsData = regs[instruction.srcReg];
            memory->memStartStore(regs[instruction.tarReg], 1, &fsData, &fsDone);
        }

        state = CPU_STATE_WAIT;
        working = 0;
    }
}

uint8_t Cpu::isMoreWorkNeeded() 
{
    return working;
}

// cpu specific functions

void Cpu::decodeInstruction(uint32_t newInstruction)
{
    instruction.inst = ((newInstruction >> CPU_INST_SHIFT) & CPU_INST_MASK);
    instruction.dest = ((newInstruction >> CPU_DEST_SHIFT) & CPU_DEST_MASK);
    instruction.srcReg = ((newInstruction >> CPU_SRC_REG_SHIFT) & CPU_SRC_REG_MASK);
    instruction.tarReg = ((newInstruction >> CPU_TAR_REG_SHIFT) & CPU_TAR_REG_MASK);
    instruction.val = ((newInstruction >> CPU_VAL_SHIFT) & CPU_VAL_MASK);
}

// register memory device to cpu device
void Cpu::registerMemory(Memory* newMemory) 
{
    memory = newMemory;
}

// register instruction memory device to cpu device
void Cpu::registerIMemory(IMemory* newIMemory) 
{
    imemory = newIMemory;
}

// sets the register <reg> to the value <val>
void Cpu::setReg(uint8_t reg, uint8_t val)
{
    if (reg == CPU_REG_PC) {
        // setting pc cancels current process
        state = CPU_STATE_FETCH; 
        working = 0;
        pc = val;
    }
    else {
        regs[reg] = val;
    }
}