#include "loop.c"
#include <cpu.h>
#include <stdio.h>

int main()
{
    FILE *logfile = fopen("logfile.txt", "w");
    if (!logfile)
    {
        perror("Failed to open logfile");
        exit(1);
    }
    Z80_State cpu;
    initCPU(&cpu);
    load_memory(cpu.mmu, "/Users/pranavdronavalli/Downloads/gb-test-roms-master/cpu_instrs/individual/03-op sp,hl.gb");
    init_opcode_table();
    execute_cycle(&cpu, logfile);
}