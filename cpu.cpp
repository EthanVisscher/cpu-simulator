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
// unhalts cpu and sets tick counter to 0
void Cpu::reset()
{
    state = CPU_STATE_IDLE;
    halted = false;
    tc = 0;

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
    printf("TC: %d\n", tc);
    printf("\n");
}

// clock client interface functions

void Cpu::startTick() 
{
    if (halted)
        return;

    tc++;
    working = 1;
    if (state == CPU_STATE_IDLE) {
        state = CPU_STATE_FETCH;
    } 
}

// called by clock to give cpu a chance to do some work
void Cpu::doCycleWork() 
{
    if (halted)
        return;

    if (state == CPU_STATE_WAIT) {
        // check if fetch or store is done
        if (fsDone) {
            if (instruction.inst == CPU_INST_LW) {
                // cache was flushing, have to wait another 5 ticks to fill cache    
                if (cache->isFlushing()) {
                    cache->memStartFetch(regs[instruction.tarReg], 1, &fsData, &fsDone);
                    state = CPU_STATE_WAIT;
                }
                else {
                    regs[instruction.dest] = cache->getWord();
                    state = CPU_STATE_IDLE;
                }
            }
            else {
                cache->setWord();
                state = CPU_STATE_IDLE;
            }
        }

        working = 0;
    }
    else if (state == CPU_STATE_WORK) {
        tickCount--;
        if (tickCount == 0) 
            state = CPU_STATE_IDLE;

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
            if (cache->isAvailableForFetch(regs[instruction.tarReg])) {
                cache->memStartFetch(regs[instruction.tarReg], 1, &fsData, &fsDone);
                if (fsDone) {
                    // data was saved in cache
                    regs[instruction.dest] = fsData;    
                    state = CPU_STATE_IDLE;
                }
                else {
                    // not cached, have to wait 5 ticks
                    state = CPU_STATE_WAIT;
                }
            }
        }
        // store word
        else if (instruction.inst == CPU_INST_SW) {
            if (cache->isAvailableForStore(regs[instruction.tarReg])) {
                fsData = regs[instruction.srcReg];
                cache->memStartStore(regs[instruction.tarReg], 1, &fsData, &fsDone);
                if (fsDone)
                    state = CPU_STATE_IDLE;
                else
                    state = CPU_STATE_WAIT;
            }
        }
        // add
        else if (instruction.inst == CPU_INST_ADD) {
            int8_t s = regs[instruction.srcReg]; 
            int8_t t = regs[instruction.tarReg]; 
            regs[instruction.dest] = s + t;
            state = CPU_STATE_IDLE;
        }
        // add immediate
        else if (instruction.inst == CPU_INST_ADDI) {
            int8_t s = regs[instruction.srcReg];
            int8_t i = instruction.val;
            regs[instruction.dest] = s + i;
            state = CPU_STATE_IDLE;
        }
        // mul
        else if (instruction.inst == CPU_INST_MUL) {
            uint8_t s = regs[instruction.srcReg];
            uint8_t high = (s >> 4) & 0b1111;
            uint8_t low = s & 0b1111;
            regs[instruction.dest] = low * high;
            tickCount = 1;      // takes 1 extra cycle
            state = CPU_STATE_WORK;
        }
        // inv
        else if (instruction.inst == CPU_INST_INV) {
            uint8_t s = regs[instruction.srcReg];
            regs[instruction.dest] = ~s;
            state = CPU_STATE_IDLE;
        }
        // branches 
        else if(instruction.inst == CPU_INST_BRANCH) {
            // beq
            if (instruction.dest == CPU_INST_BEQ) {
                uint8_t s = regs[instruction.srcReg];
                uint8_t t = regs[instruction.tarReg];
                if (t == s) {
                    pc = instruction.val;
                    tickCount = 1;      // takes 1 extra cycle
                    state = CPU_STATE_WORK;
                }
                else {
                    state = CPU_STATE_IDLE;
                }
            }
            // bneq
            else if (instruction.dest == CPU_INST_BNEQ) {
                uint8_t s = regs[instruction.srcReg];
                uint8_t t = regs[instruction.tarReg];
                if (t != s) {
                    pc = instruction.val;
                    tickCount = 1;      // takes 1 extra cycle
                    state = CPU_STATE_WORK;
                }
                else {
                    state = CPU_STATE_IDLE;
                }
            }
            // blt
            else if (instruction.dest == CPU_INST_BLT) {
                int8_t s = regs[instruction.srcReg];
                int8_t t = regs[instruction.tarReg];
                if (s < t) {
                    pc = instruction.val;
                    tickCount = 1;      // takes 1 extra cycle
                    state = CPU_STATE_WORK;
                }
                else {
                    state = CPU_STATE_IDLE;
                }
            }
        }
        // halt
        else if (instruction.inst == CPU_INST_HALT) {
            halted = true;
        }

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

void Cpu::registerCache(Cache* newCache)
{
    cache = newCache;
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