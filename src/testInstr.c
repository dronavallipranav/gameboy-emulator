#include "loop.c"
#include <cpu.h>
#include <stdio.h>

int main(){
    Z80_State cpu;
    initCPU(&cpu);
    init_opcode_table();
    execute_cycle(&cpu);
    
}