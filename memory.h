#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>

#include "parse_client.h"
#include "clock_client.h"

using namespace std;

#define MEM_STATE_IDLE  0
#define MEM_STATE_WAIT  1
#define MEM_STATE_MOVE  2

class Memory : public ParseClient, public ClockClient 
{
    public:
        Memory();
        ~Memory();
        void memStartFetch(unsigned int addr, unsigned int count,
                            uint8_t *dataPtr, uint8_t *memDonePtr);

        // parse client interface functions
        void parse(ifstream& infile, string command);
        void reset();
        void dump(ifstream& infile);

        // clock client interface functions
        void startTick();
        void doCycleWork();
        void isMoreWorkNeeded();

    private:
        uint8_t state;
        uint8_t* memPtr;
        size_t size;        

        void create(size_t createSize);
        void set(unsigned int offset, uint8_t val);
};

#endif