#include "iodev.h"

// IODev class constructor
IODev::IODev()
{
    reset();
    ec = 0;
    tc = 0;
    state = IODEV_STATE_IDLE;
}

// parse client interface functions

// prase commands from file stream
void IODev::parse(ifstream& infile, string command)
{
    if (command == "load") {
        infile >> command;
        load(command);
    }
}

// set register to 0
void IODev::reset()
{
    reg = 0;
}

// dump register contents
void IODev::dump(ifstream& infile)
{
    printf("IO Device: 0x%02X\n\n", reg);
}

// clock client interface functions

void IODev::startTick()
{
    working = 1;

    /*
    // scheduled event must have been skipped because memory was occupied
    if (state == IODEV_STATE_IDLE && events[ec].tick < tc)
        ec++;
    */

    if (state == IODEV_STATE_IDLE && events[ec].tick == tc)
        state = IODEV_STATE_EXEC;
}

void IODev::doCycleWork()
{
    if (working == 0)
        return;

    if (state == IODEV_STATE_EXEC) {
        // if memory isn't available, try to execute next cycle
        if (memory->isAvailable()) {
            if (events[ec].func == IODEV_FUNC_READ)
                memory->memStartFetch(events[ec].addr, 1, &reg, &fsDone);
            else
                memory->memStartStore(events[ec].addr, 1, &events[ec].val, &fsDone);

            state = IODEV_STATE_WAIT;
        }
    }
    else if (state == IODEV_STATE_WAIT) {
        if (fsDone) {
            state = IODEV_STATE_IDLE;
            ec++;        
        }
    }

    tc++;
    working = 0;
}

uint8_t IODev::isMoreWorkNeeded()
{
    return working;
}

// iodev specific functions

void IODev::registerMemory(Memory* newMemory)
{
    memory = newMemory;
}

// load in schedule events to events array
void IODev::load(string filename)
{
    uint8_t i = 0;
    ifstream scheduledEvents(filename);

    if (!scheduledEvents)
        return;

    string word;
    scheduledEvents >> word;
    while (scheduledEvents) {
        events[i].tick = stoi(word);

        scheduledEvents >> word;
        if (word == "read") {
            events[i].func = IODEV_FUNC_READ;

            scheduledEvents >> word;
            events[i].addr = stoi(word, 0, 16);
        }
        else {
            events[i].func = IODEV_FUNC_WRITE;

            scheduledEvents >> word;
            events[i].addr = stoi(word, 0, 16);

            scheduledEvents >> word;
            events[i].val = stoi(word, 0, 16);
        }

        i++;
        scheduledEvents >> word;
    }
}
