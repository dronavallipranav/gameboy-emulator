#include <cpu.h>

void loadImm(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t imm);
//TO DO: Create Function pointers for each register to pass in as params
void loadReg(Z80_State *cpu,  uint8_t opcode);