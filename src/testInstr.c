#include "loop.c"
#include <cpu.h>
#include <stdio.h>

int main()
{
    Z80_State cpu;
    initCPU(&cpu);
    load_memory(cpu.mmu, "/Users/pranavdronavalli/Downloads/gb-test-roms-master/cpu_instrs/individual/01-special.gb");
    init_opcode_table();
    execute_cycle(&cpu);
}