#include <extended.h>
#include <stdint.h>
#include <cpu.h>

void swap(Z80_State* cpu, uint8_t *bits){
    uint8_t low = *bits & 0x0F;
    uint8_t high = *bits & 0xF0;
    low = low << 4;
    high = high >> 4;
    *bits = low | high;
    cpu->AF.flags.Z = *bits == 0 ? 1 : 0;
}