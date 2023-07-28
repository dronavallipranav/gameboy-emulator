#include <stdint.h>
#include <cpu.h>

void swap(Z80_State *cpu, uint8_t *bits);

void handler_cp(Z80_State *cpu, uint8_t opcode);