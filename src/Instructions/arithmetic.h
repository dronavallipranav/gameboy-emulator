#include <stdint.h>
#include "cpu.h"

void add8(Z80_State *cpu, uint8_t reg);

void add16(Z80_State *cpu, uint16_t reg);

void addImm(Z80_State *cpu, uint16_t reg);

void sub8(Z80_State *cpu, uint8_t reg);

void sub16(Z80_State *cpu ,uint16_t reg);

void subImm(Z80_State *cpu, uint16_t reg);

