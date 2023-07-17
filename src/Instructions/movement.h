#include <cpu.h>

void loadImm(Z80_State *cpu, uint8_t imm);
//TO DO: Create Function pointers for each register to pass in as params
void loadReg(Z80_State *cpu);