#include "clock.h"

// Clock class constructor
Clock::Clock() 
{
    reset();
}

// parse commands from file stream
void Clock::parse(ifstream& infile) 
{
    string command;
    infile >> command;

    if (command == "reset") {
        reset();
    } 
    else if (command == "tick") {
        infile >> command;
        uint16_t ticks = stoi(command);
        tick(ticks);
    }
    else if (command == "dump") {
        dump();
    }
}

// register a cpu to the clock device
void Clock::registerClient(ClockClient* client) {
    clients[numOfClients] = client;
    numOfClients++;
}

// resets internal counter to 0
void Clock::reset()
{
    counter = 0;
}

// internal counter incremented by <ticks> ticks
void Clock::tick(uint16_t ticks) 
{
    for (int i = 1; i <= ticks; i++) {
        //cpu->doCycleWork();
        counter += 1;
    }
}

// dumps value of internal counter
void Clock::dump()
{
    printf("Clock: %d\n", counter);
    printf("\n");
}