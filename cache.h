#ifndef _CACHE_H
#define _CACHE_H

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

#define CACHE_NUM_OF_BYTES  8

class Cache : public ParseClient
{
    public:
        Cache();
        void registerMemory(Memory* newMemory);

        void memStartFetch(unsigned int offset, unsigned int count,
                            uint8_t *dataPtr, uint8_t *memDonePtr);
        void memStartStore(unsigned int offset, unsigned int count,
                            uint8_t *dataPtr, uint8_t *memDonePtr);
        uint8_t getWord();
        void setWord();
        bool isFlushing();

        // parse client interface functions
        void parse(ifstream& infile, string command);
        void reset();
        void dump(ifstream& infile);

    private:
        bool off;               // device is turned off
        uint8_t clo;            // cache line offset
        uint8_t data[8];
        uint8_t states[8];
        bool flushing;
        Memory* memory;

        // variables to hold information on cache miss
        uint8_t loadedOffset;
        uint8_t loadedCLO;
        uint8_t loadedWordIdx;
        uint8_t loadedWord;
        uint8_t loadedStates[8];

        bool hit(unsigned int addr);
        bool validData(unsigned int addr);
        unsigned int getCacheIndex(unsigned int addr);
        void flush(uint8_t *memDonePtr);
};

#endif