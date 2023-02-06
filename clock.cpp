#include "clock.h"

// Clock class constructor
Clock::Clock() 
{
    reset();
}

// parse client interface functions

// parse commands from file stream
void Clock::parse(ifstream& infile, string command) 
{
    if (command == "tick") {
        infile >> command;
        uint16_t ticks = stoi(command);
        tick(ticks);
    }
}

// resets internal counter to 0
void Clock::reset()
{
    counter = 0;
}

// dumps value of internal counter
void Clock::dump(ifstream& infile)
{
    printf("Clock: %d\n", counter);
    printf("\n");
}

// clock specific functions

// register a cpu to the clock device
void Clock::registerClient(ClockClient* client) {
    clients[numOfClients] = client;
    numOfClients++;
}

// internal counter incremented by <ticks> ticks
void Clock::tick(uint16_t ticks) 
{
    for (int i = 1; i <= ticks; i++) {
        for (int j = 0; j < numOfClients; j++)
            clients[j]->doCycleWork();

        counter += 1;
    }
}