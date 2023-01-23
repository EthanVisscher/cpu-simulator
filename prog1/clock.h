#ifndef _CLOCK_H
#define _CLOCK_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

#include <stdint.h>
#include <cstdio>

#include "cpu.h"

using namespace std;

class Clock 
{
    public:
        Clock();
        void parse(ifstream& infile);
        void registerCpu(Cpu* newCpu);

    private:
        uint16_t counter;
        Cpu* cpu;           // cpu registered to clock

        void reset();
        void tick(uint16_t ticks);
        void dump();
};

#endif