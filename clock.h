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

        void parse(ifstream& infile);

    private:
        uint16_t counter;
        ClockClient* clients[10];  // devices registered to clock
        uint8_t numOfClients = 0;

        void reset();
        void tick(uint16_t ticks);
        void dump();
};

#endif