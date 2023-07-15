#include "cpu.h"
#include <arithmetic.h>

void add8(Z80_State *cpu, uint8_t reg){
    cpu -> A += reg;
}