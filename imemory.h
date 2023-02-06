#ifndef _IMEMORY_H
#define _IMEMORY_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>

#include "parse_client.h"

using namespace std;

class IMemory : public ParseClient
{
    public:
        IMemory();
        ~IMemory();
        void memStartFetch(unsigned int addr, unsigned int count,
                            uint32_t *dataPtr, uint32_t *memDonePtr);

        // parse client interface functions
        void parse(ifstream& infile, string command);
        void reset();
        void dump(ifstream& infile);

    private:
        uint32_t* memPtr;
        size_t size;

        void create(size_t createSize);
        void set(unsigned int offset, uint32_t val);
};

#endif