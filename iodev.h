#ifndef _IODEV_H
#define _IODEV_H

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

#define IODEV_MAX_EVENTS    100

#define IODEV_FUNC_READ     1 
#define IODEV_FUNC_WRITE    2 

#define IODEV_STATE_IDLE    1
#define IODEV_STATE_EXEC    2
#define IODEV_STATE_WAIT    3

class IODev : public ParseClient, public ClockClient
{
    public:
        typedef struct {
            unsigned int tick;
            unsigned int func;
            unsigned int addr;
            uint8_t val;
        } Event;
        
        IODev();
        void registerMemory(Memory* newMemory);
        
        // parse client interface functions
        void parse(ifstream& infile, string command);
        void reset();
        void dump(ifstream& infile);

        // clock client interface functions
        void startTick();
        void doCycleWork();
        uint8_t isMoreWorkNeeded();

    private:
        uint8_t reg;
        Event events[IODEV_MAX_EVENTS];
        uint8_t state;
        uint8_t working;
        uint8_t tc;     // tick count
        uint8_t ec;     // events count
        Memory* memory;

        uint8_t fsDone;

        void load(string filename);
};

#endif