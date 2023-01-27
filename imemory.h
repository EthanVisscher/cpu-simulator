#ifndef _IMEMORY_H
#define _IMEMORY_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

#include <stdint.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

class IMemory 
{
    public:
        IMemory();
        ~IMemory();
        void memStartFetch(unsigned int addr, unsigned int count,
                            uint32_t *dataPtr, uint32_t *memDonePtr);
        void parse(ifstream& infile);

    private:
        uint32_t* memPtr;
        size_t size;
        void reset();
        void create(size_t createSize);
        void set(unsigned int offset, uint32_t val);
        void dump(unsigned int offset, unsigned int count);
};

#endif