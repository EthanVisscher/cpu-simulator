#ifndef _CLOCK_H
#define _CLOCK_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <cstdio>

#include "cpu.h"
#include "parse_client.h"

using namespace std;

class Clock : public ParseClient
{
    public:
        Clock();
        void registerClient(ClockClient* client);

        // clock client inerface functions
        void parse(ifstream& infile, string command);
        void reset();
        void dump(ifstream& infile);

    private:
        uint16_t counter;          // current tick
        ClockClient* clients[10];  // devices registered to clock
        uint8_t numOfClients = 0;

        void tick(uint16_t ticks);
};

#endif