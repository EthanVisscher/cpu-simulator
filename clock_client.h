#ifndef _CLOCK_CLIENT_H
#define _CLOCK_CLIENT_H

#include <stdint.h>

class ClockClient
{
    public:
        virtual void startTick() = 0;           // start of a new cycle
        virtual void doCycleWork() = 0;         // given a chance to do work
        virtual uint8_t isMoreWorkNeeded() = 0; // check if all work is done
};

#endif