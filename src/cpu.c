#include <stdint.h>
#include <stdio.h>
#include "../include/cpu.h"
#include <stdlib.h>
#include <string.h>

void initCPU(Z80_State *cpu){
    printf("HelloCPU\n");
    //dynamically allocate and init memory space to 0
    memset(cpu->memory, 0, sizeof(cpu->memory));

    // Initialize registers to zero
    cpu->A = 0;
    cpu->B = 0;
    cpu->C = 0;
    cpu->D = 0;
    cpu->E = 0;
    cpu->H = 0;
    cpu->L = 0;
    cpu->I = 0;
    cpu->R = 0;
    cpu->F = 0;
    cpu->IX = 0;
    cpu->IY = 0;
    cpu->SP = 0;
    cpu->PC = 0;
    cpu->interrupt = 0;

    // Initialize flags to zero
    cpu->flags.S = 0;
    cpu->flags.Z = 0;
    cpu->flags.fifth = 0;
    cpu->flags.H = 0;
    cpu->flags.third = 0;
    cpu->flags.PV = 0;
    cpu->flags.N = 0;
    cpu->flags.C = 0;

}

