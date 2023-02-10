#ifndef _CPU_H
#define _CPU_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <cstdio>

#include "memory.h"
#include "imemory.h"
#include "parse_client.h"
#include "clock_client.h"

using namespace std;

#define CPU_NUM_OF_REGS 8
#define CPU_REG_RA      0
#define CPU_REG_RB      1
#define CPU_REG_RC      2
#define CPU_REG_RD      3
#define CPU_REG_RE      4
#define CPU_REG_RF      5
#define CPU_REG_RG      6
#define CPU_REG_RH      7
#define CPU_REG_PC      9   // 9 has no significance 

#define CPU_STATE_IDLE  1
#define CPU_STATE_FETCH 2
#define CPU_STATE_WAIT  3
#define CPU_STATE_EXEC  4

#define CPU_INST_MASK       0b111
#define CPU_DEST_MASK       0b111
#define CPU_SRC_REG_MASK    0b111
#define CPU_TAR_REG_MASK    0b111
#define CPU_VAL_MASK        0b11111111

#define CPU_INST_SHIFT      17 
#define CPU_DEST_SHIFT      14 
#define CPU_SRC_REG_SHIFT   11 
#define CPU_TAR_REG_SHIFT   8 
#define CPU_VAL_SHIFT       0 

#define CPU_INST_LW     0b101
#define CPU_INST_SW     0b110

class Cpu : public ParseClient, public ClockClient
{
    public:
        typedef struct {
            unsigned int inst : 3;      // instruction encoding
            unsigned int dest : 3;      // destination register selector
            unsigned int srcReg : 3;    // source register selector
            unsigned int tarReg : 3;    // target register selector
            unsigned int val : 8;       // immediate value
        } Instruction;

        Cpu();
        void registerMemory(Memory* newMemory);
        void registerIMemory(IMemory* newIMemory);

        // parse client interface functions
        void parse(ifstream& infile, string command);
        void reset();
        void dump(ifstream& infile);

        // clock client interface functions
        void startTick();
        void doCycleWork();
        uint8_t isMoreWorkNeeded();

    private:
        uint8_t state;      // current state of CPU
        uint8_t working;    // currently working in cycle
        uint8_t pc;         // program counter
        uint8_t regs[8];    // registers RA-RH
        Memory* memory;     // data memory
        IMemory* imemory;   // instruction memory
        Instruction instruction;


        // variables for fetching and storing to memory
        uint8_t fsData; 
        uint8_t fsDone; 

        void decodeInstruction(uint32_t newInstruction);
        void setReg(uint8_t reg, uint8_t val);
};

#endif