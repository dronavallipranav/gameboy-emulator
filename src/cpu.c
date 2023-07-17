#include <stdint.h>
#include <stdio.h>
#include "../include/cpu.h"
#include <stdlib.h>
#include <string.h>

void initCPU(Z80_State *cpu){
  
    //init memory space to 0
    memset(cpu->memory, 0, sizeof(cpu->memory));

    // Initialize registers to zero
    cpu->AF.A = 0;
    cpu -> AF.F = 0;
    cpu->BC.B = 0;
    cpu->BC.C = 0;
    cpu->DE.D = 0;
    cpu->DE.E = 0;
    cpu->HL.H = 0;
    cpu->HL.L = 0;
    cpu->I = 0;
    cpu->R = 0;
    cpu->IX = 0;
    cpu->IY = 0;
    cpu->SP = 0;
    cpu->PC = 0;
    cpu->interrupt = 0;

    // Initialize flags to zero
    cpu->AF.flags.S = 0;
    cpu->AF.flags.Z = 0;
    cpu->AF.flags.fifth = 0;
    cpu->AF.flags.H = 0;
    cpu->AF.flags.third = 0;
    cpu->AF.flags.PV = 0;
    cpu->AF.flags.N = 0;
    cpu->AF.flags.C = 0;

}

