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
        // let clients perpare for cycle
        for (int j = 0; j < numOfClients; j++)
            clients[j]->startTick();

        uint8_t working = 1;
        while (working > 0) {
            for (int j = 0; j < numOfClients; j++)
                clients[j]->doCycleWork();

            // poll to see if any clients still need to work
            working = 0;
            for (int j = 0; j < numOfClients; j++)
                working += clients[j]->isMoreWorkNeeded();
        }

        counter += 1;
    }
}