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

#define MEM_STATE_IDLE  1
#define MEM_STATE_WAIT  2
#define MEM_STATE_MOVE  3

#define WAIT_ON_FETCH   1
#define WAIT_ON_STORE   2
#define MEM_FS_SPEED    4

class Memory : public ParseClient, public ClockClient 
{
    public:
        Memory();
        ~Memory();
        void memStartFetch(unsigned int offset, unsigned int count,
                            uint8_t *dataPtr, uint8_t *memDonePtr);
        void memStartStore(unsigned int offset, unsigned int count,
                            uint8_t *dataPtr, uint8_t *memDonePtr);

        // parse client interface functions
        void parse(ifstream& infile, string command);
        void reset();
        void dump(ifstream& infile);

        // clock client interface functions
        void startTick();
        void doCycleWork();
        uint8_t isMoreWorkNeeded();

    private:
        uint8_t state;          // state of memory
        uint8_t working;        // currently working in cycle
        uint8_t* memPtr;        // malloc'd memory segment
        size_t size;            // size of memory segment

        // variables for fetching and storing called by CPU
        uint8_t fsTickCount;    // needs 5 ticks to complete
        unsigned int fsOffset;  // offset of fetch or store
        unsigned int fsCount;   // size of fetch or store
        uint8_t* fsDataPtr;  
        uint8_t* fsDonePtr;
        uint8_t fsType;        // declares current wait as fetch or store

        void setFsInfo(unsigned int offset, unsigned int count,
                        uint8_t *dataPtr, uint8_t *memDonePtr);
        void completeFetch();
        void completeStore();
        void create(size_t createSize);
        void set(unsigned int offset, uint8_t val);
};

#endif