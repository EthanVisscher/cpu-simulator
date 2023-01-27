#ifndef _CPU_H
#define _CPU_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

#include <stdint.h>
#include <cstdio>

#include "memory.h"

using namespace std;

#define PC  0
#define RA  1
#define RB  2
#define RC  3
#define RD  4
#define RE  5
#define RF  6
#define RG  7
#define RH  8

class Cpu
{
    public:
        Cpu();
        void doCycleWork();
        void parse(ifstream& infile);
        void registerMemory(Memory* newMemory);

    private:
        // program counter and registers RA through RH
        uint8_t regs[9];
        Memory* memory;

        void reset();
        void setReg(uint8_t reg, uint8_t val);
        void dump();
};

#endif