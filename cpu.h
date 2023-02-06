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

#define NUM_OF_REGS 8

class Cpu : public ParseClient, public ClockClient
{
    public:
        enum CpuReg {RA, RB, RC, RD, RE, RF, RG, RH, PC};
        enum CpuState {Idle, Fetch, Waiting, Executing};

        Cpu();
        void registerMemory(Memory* newMemory);

        void parse(ifstream& infile);
        void startTick();
        void doCycleWork();
        void isMoreWorkNeeded();

    private:
        // program counter and registers RA through RH
        uint8_t pc;
        uint8_t regs[8];
        CpuState state;
        Memory* memory;

        void reset();
        void setReg(CpuReg reg, uint8_t val);
        void dump();
};

#endif