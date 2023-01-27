#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

#include "cpu.h"
#include "memory.h"
#include "clock.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    // open file stream
    if (argc < 2) {
        return 1;
    }

    ifstream infile(argv[1]);
    if (!infile) {
        return 1;
    }

    // create device objects
    Cpu* cpu = new Cpu();
    Clock* clock = new Clock();
    Memory* memory = new Memory();

    clock->registerCpu(cpu);
    cpu->registerMemory(memory);

    // start parsing file
    string device;
    infile >> device;
    while (infile) {
        if (device == "cpu") {
            cpu->parse(infile);
        }
        else if (device == "clock") {
            clock->parse(infile);
        }
        else if (device == "memory") {
            memory->parse(infile);
        }

        infile >> device;
    }

    delete cpu;
    delete clock;
    delete memory;
    return 0; 
}