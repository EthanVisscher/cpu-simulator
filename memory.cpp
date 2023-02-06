#include "memory.h"

// Memory class constructor
Memory::Memory()
{
    size = 0;
}

// Memory class destructor
Memory::~Memory() 
{
    free(memPtr);
}

// parse client interface functions

// parse commands from file stream
void Memory::parse(ifstream& infile, string command) 
{
    if (command == "create") {
        infile >> command;
        unsigned int createSize = (unsigned int)stoi(command, 0, 16);
        create(createSize);
    } 
    else if (command == "set") {
        infile >> command;
        unsigned int offset = stoi(command, 0, 16);

        infile >> command;
        unsigned int count = stoi(command, 0, 16);

        for (unsigned int i = 0; i < count; i++) {
            infile >> command;
            uint8_t val = stoi(command, 0, 16);
            set(offset + i, val);
        }
    }
}

// reset memory to all 0's
void Memory::reset()
{
    memset(memPtr, 0, size);
}

// dump out <count> bytes of memory starting at <offset>
void Memory::dump(ifstream& infile)
{    
    unsigned int offset = 0;
    unsigned int count = 0;

    string command;
    infile >> command;
    offset = stoi(command, 0, 16);
    infile >> command;
    count = stoi(command, 0, 16);

    // out of bounds
    if (offset + count > size) {
        return;
    }

    unsigned int firstDigit = (offset & 0xF0) >> 4;
    unsigned int lastDigit = (offset & 0x0F);

    // calculate amount of rows to  print, not including first row
    unsigned int rows = 0;
    rows += (offset + count) / 16;
    if ((offset + count) % 16 > 0) {
        rows += 1;
    }

    printf("Addr 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
    unsigned int k = 0;
    for (unsigned int i = 0; i < rows; i++) {
        printf("0x%02X ", firstDigit);

        for (unsigned int j = 0; j < 16; j++) {
            // need to start at an offset
            if (i == 0 && j < lastDigit) {
                printf("   ");
            }
            // done printing
            else if (k >= count) {
                printf("\n");
                return;
            }
            else {
                printf("%02X ", *(memPtr + offset + k));
                k += 1;
            }

        }

        printf("\n");
        firstDigit += 0x10;
    }
    printf("\n");
}

// clock client inerface functions

void Memory::startTick() 
{

}

void Memory::doCycleWork()
{

}

void Memory::isMoreWorkNeeded()
{

}

// memory specific functions

// api called to initiate fetch or writes from or to memory
void Memory::memStartFetch(unsigned int offset, unsigned int count,
                            uint8_t *dataPtr, uint8_t *memDonePtr)
{
    if (count == 1) {
        *dataPtr = memPtr[offset];
    }
    else {
        memcpy(dataPtr, memPtr + offset, count);
    }

    *memDonePtr = 1;
}

// create memory segment
void Memory::create(size_t createSize)
{
    size = createSize;
    memPtr = (uint8_t*)malloc(size);
}


// set byte of memory at <offset> to <val>
void Memory::set(unsigned int offset, uint8_t val)
{
    *(memPtr + offset) = val;
}
