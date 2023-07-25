#include <stdint.h>
#include <cpu.h>
#include <stdbool.h>
void add8(Z80_State *cpu, uint8_t reg, bool carryStatus);

void add16(Z80_State *cpu, uint16_t reg);

void addImm(Z80_State *cpu, uint16_t imm);

void sub8(Z80_State *cpu, uint8_t reg, bool carryStatus);

void sub16(Z80_State *cpu ,uint16_t reg);

void subImm(Z80_State *cpu, uint16_t imm);

void and8(Z80_State *cpu, uint8_t reg);

void or8(Z80_State *cpu, uint8_t reg);

void xor8(Z80_State *cpu, uint8_t reg);

void cp8(Z80_State *cpu, uint8_t reg);

void inc8(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t val);

void dec8(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t val);

void ALU(Z80_State *cpu, uint8_t opcode);
