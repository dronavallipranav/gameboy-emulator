#include "loop.c"
#include <cpu.h>
#include <stdio.h>

int main(){
    Z80_State cpu;
    initCPU(&cpu);
    execute_cycle(&cpu);
    printf("yay");
}