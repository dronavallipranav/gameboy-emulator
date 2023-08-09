#include <stdint.h>
#include <cpu.h>

void swap(Z80_State *cpu, uint8_t *bits);

void handler_cb(Z80_State *cpu, uint8_t opcode);