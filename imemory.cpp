#include "imemory.h"

// Memory class constructor
IMemory::IMemory()
{
    size = 0;
}

// Memory class destructor
IMemory::~IMemory() 
{
    free(memPtr);
}

// api called to initiate fetch or writes from or to memory
void IMemory::memStartFetch(unsigned int offset, unsigned int count,
                            uint32_t *dataPtr, uint32_t *memDonePtr)
{
    if (count == 1) {
        *dataPtr = memPtr[offset];
    }
    else {
        memcpy(dataPtr, memPtr + offset, count);
    }

    *memDonePtr = 1;
}

// parse commands from file stream
void IMemory::parse(ifstream& infile) 
{
    string command;
    infile >> command;

    if (command == "create") {
        infile >> command;
        unsigned int createSize = (unsigned int)stoi(command, 0, 16);
        create(createSize);
    } 
    else if (command == "reset") {
        reset();
    }
    else if (command == "dump") {
        infile >> command;
        unsigned int offset = stoi(command, 0, 16);

        infile >> command;
        unsigned int count = stoi(command, 0, 16);

        dump(offset, count);
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

// create memory segment
void IMemory::create(size_t createSize)
{
    size = createSize;
    memPtr = (uint32_t*)malloc(size);
}

// reset memory to all 0's
void IMemory::reset()
{
    memset(memPtr, 0, size);
}

// set byte of memory at <offset> to <val>
void IMemory::set(unsigned int offset, uint32_t val)
{
    *(memPtr + offset) = val;
}

// dump out <count> bytes of memory starting at <offset>
void IMemory::dump(unsigned int offset, unsigned int count)
{
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

    printf("Addr     0     1     2     3     4      5     6     7\n");
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