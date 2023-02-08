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

// parse commands from file stream
void IMemory::parse(ifstream& infile, string command) 
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

        if (command == "file") {
            infile >> command;
            ifstream instructions(command); 
            if (!infile)
                return;

            string instructionText;
            unsigned int i = 0;
            while(instructions) {
                instructions >> instructionText;
                uint32_t instruction = stoi(instructionText, 0, 16);
                set(offset + i, instruction);
                i++;
            }
        }
        else {
            infile >> command;
            unsigned int count = stoi(command, 0, 16);

            for (unsigned int i = 0; i < count; i++) {
                infile >> command;
                uint32_t val = stoi(command, 0, 16);
                set(offset + i, val);
            }
        }
    }
}

// reset memory to all 0's
void IMemory::reset()
{
    memset(memPtr, 0, size);
}

// dump out <count> bytes of memory starting at <offset>
void IMemory::dump(ifstream& infile)
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

    unsigned int row = offset - (offset % 8);
    unsigned int startCol = offset % 8;

    // calculate amount of rows to  print, not including first row
    unsigned int rows = 0;
    rows += (offset + count) / 8;
    if ((offset + count) % 8 > 0) {
        rows += 1;
    }

    printf("Addr     0     1     2     3     4     5     6     7\n");
    unsigned int k = 0;
    for (unsigned int i = 0; i < rows; i++) {
        printf("0x%02X ", row);

        for (unsigned int j = 0; j < 8; j++) {
            // need to start at an offset
            if (i == 0 && j < startCol) {
                printf("      ");
            }
            // done printing
            else if (k >= count) {
                printf("\n\n");
                return;
            }
            else {
                printf("%05X ", *(memPtr + offset + k));
                k += 1;
            }

        }

        printf("\n");
        row += 8;
    }
    printf("\n");
}

// imemory specific functions

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

// create memory segment
void IMemory::create(size_t createSize)
{
    size = createSize;
    memPtr = (uint32_t*)malloc(size);
}

// set byte of memory at <offset> to <val>
void IMemory::set(unsigned int offset, uint32_t val)
{
    *(memPtr + offset) = val;
}
