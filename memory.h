#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

#include <stdint.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

class Memory 
{
    public:
        Memory();
        ~Memory();
        void memStartFetch(unsigned int addr, unsigned int count,
                            uint8_t *dataPtr, uint8_t *memDonePtr);
        void parse(ifstream& infile);

    private:
        uint8_t* memPtr;
        size_t size;
        void reset();
        void create(size_t createSize);
        void set(unsigned int offset, uint8_t val);
        void dump(unsigned int offset, unsigned int count);
};

#endif