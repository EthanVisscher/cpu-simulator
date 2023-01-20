#include <stdio.h>
#include "cpu.h"
#include "memory.h"
#include "clock.h"

int main() {
    FILE* file;
    file = fopen("samples/Sample1_input.txt", "r");

    char* device;
    int number;
    fscanf(file, "%d", &number);

    printf("%d", number);

    return 0; 
}