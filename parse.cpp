#include "parse.h"

int main(int argc, char* argv[]) 
{
    // open file stream
    if (argc < 2)
        return 1;

    ifstream infile(argv[1]);
    if (!infile)
        return 1;

    // create device objects
    Cpu* cpu = new Cpu();
    Clock* clock = new Clock();
    Memory* memory = new Memory();
    IMemory* imemory = new IMemory();
    Cache* cache = new Cache();

    clock->registerClient(cpu);
    clock->registerClient(memory);
    cpu->registerMemory(memory);
    cpu->registerIMemory(imemory);
    cpu->registerCache(cache);
    cache->registerMemory(memory);

    // start parsing file
    ParseClient* device;
    string command;
    infile >> command;  // get device name
    while (infile) {
        if (command == "cpu")
            device = cpu;
        else if (command == "clock")
            device = clock;
        else if (command == "memory")
            device = memory;
        else if (command == "imemory")
            device = imemory;
        else if (command == "cache")
            device = cache;

        infile >> command;  // get next command
        if (command == "reset")
            device->reset();
        else if (command == "dump")
            device->dump(infile);
        else
            device->parse(infile, command);

        infile >> command;  // get next name
    }

   return 0; 
}