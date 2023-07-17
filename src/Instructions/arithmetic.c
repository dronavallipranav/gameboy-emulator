#include <cpu.h>
#include <arithmetic.h>

void add8(Z80_State *cpu, uint8_t reg){
    cpu -> AF.A += reg;
}

void add16(Z80_State *cpu, uint16_t reg){
    cpu -> AF.A += reg;
}

void addImm(Z80_State *cpu, uint16_t imm){
    cpu -> AF.A += imm;
}

void sub8(Z80_State *cpu, uint8_t reg){
    cpu -> AF.A -= reg;
}

void sub16(Z80_State *cpu, uint16_t reg){
    cpu -> AF.A -= reg;
}

void subImm(Z80_State *cpu, uint16_t imm){
    cpu -> AF.A -= imm;
}


