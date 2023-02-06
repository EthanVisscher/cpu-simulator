#ifndef _CLOCK_CLIENT_H
#define _CLOCK_CLIENT_H

class ClockClient
{
    public:
        virtual void startTick() = 0;       // start of a new cycle
        virtual void doCycleWork() = 0;     // given a chance to do work
        virtual void isMoreWorkNeeded() = 0; // check if all work is done
};

#endif