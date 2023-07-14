#include <stdint.h>
#include <stdio.h>
#include <cpu.h>

int main(){

  Z80_State *cpu;
  initCPU(cpu);

}

void initCPU(Z80_State *cpu){
    //dynamically allocate and init memory space to 0
    cpu -> memory = calloc(0,MEMORY_SIZE);
    if(cpu -> memory == NULL){
      printf("unable to allocate space for memory space");
    }
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

