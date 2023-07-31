#include <cpu.h>

void daa(Z80_State *cpu) {
    uint8_t correction = cpu->AF.flags.C ? 0x60 : 0x00;
    if (cpu->AF.flags.H || (!cpu->AF.flags.N && (cpu->AF.A & 0x0F) > 9)) {
        correction |= 0x06;
    }
    if (cpu->AF.flags.C || (!cpu->AF.flags.N && cpu->AF.A > 0x99)) {
        correction |= 0x60;
    }
    cpu->AF.A = cpu->AF.flags.N ? cpu->AF.A - correction : cpu->AF.A + correction;
    cpu->AF.flags.C = (correction >= 0x60) ? 1 : 0;
    cpu->AF.flags.H = 0;
    cpu->AF.flags.Z = (cpu->AF.A == 0) ? 1 : 0;
}

void cpl(Z80_State *cpu){
    cpu->AF.A = ~(cpu->AF.A);
    cpu->AF.flags.Z = cpu->AF.A == 0 ? 1 : 0;
    cpu->AF.flags.N = 1;
    cpu->AF.flags.H = 1;
}

void ccf(Z80_State *cpu){
    if(cpu->AF.flags.C == 1)
        cpu->AF.flags.C = 0;
    else
        cpu->AF.flags.C = 1;
    
    cpu->AF.flags.H = 0;
    cpu->AF.flags.N = 0;
}

void scf(Z80_State *cpu){
    cpu->AF.flags.C = 1;
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = 0;
}

handle_manip(Z80_State *cpu, uint8_t opcode){
    switch (opcode){
    case 0x27:
        daa(cpu);
        break;
    case 0x2F:
        cpl(cpu);
        break;
    case 0x3F:
        ccf(cpu);
        break;
    case 0x37:
        scf(cpu);
        break;
    }
}