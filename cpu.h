#ifndef _CPU_H
#define _CPU_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <cstdio>

#include "memory.h"
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

#define CPU_STATE_IDLE  0
#define CPU_STATE_FETCH 1
#define CPU_STATE_WAIT  2
#define CPU_STATE_EXEC  3

class Cpu : public ParseClient, public ClockClient
{
    public:
        enum CpuReg {RA, RB, RC, RD, RE, RF, RG, RH, PC};
        enum CpuState {Idle, Fetch, Waiting, Executing};

        Cpu();
        void registerMemory(Memory* newMemory);

        // parse client interface functions
        void parse(ifstream& infile, string command);
        void reset();
        void dump(ifstream& infile);

        // clock client interface functions
        void startTick();
        void doCycleWork();
        void isMoreWorkNeeded();

    private:
        // program counter and registers RA through RH
        uint8_t pc;
        uint8_t regs[8];
        uint8_t state;
        Memory* memory;

        void setReg(uint8_t reg, uint8_t val);
};

#endif