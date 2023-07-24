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

void ALU(Z80_State *cpu, uint8_t opcode){
    uint8_t val;
    uint16_t addr;
    uint8_t (*getReg)(Z80_State *);
    uint16_t (*getReg)(Z80_State *);
    switch(opcode){
        case 0x87:
        val = cpu->AF.A;
        add8(cpu, val);
        break;
    case 0x80:
        val = cpu->BC.B;
        add8(cpu, val);
        break;
    case 0x81:
        val = cpu->BC.C;
        add8(cpu, val);
        break;
    case 0x82:
        val = cpu->DE.D;
        add8(cpu, val);
        break;
    case 0x83:
        val = cpu->DE.E;
        add8(cpu, val);
        break;
    case 0x84:
        val = cpu->HL.H;
        add8(cpu, val);
        break;
    case 0x85:
        val = cpu->HL.L;
        add8(cpu, val);
        break;
    case 0x86:
        addr = (cpu->HL.H << 8) | cpu->HL.L; 
        val = cpu->memory[addr];
        add8(cpu, val);
        break;
    case 0xC6:
        val = cpu->memory[cpu->PC+1];
        add8(cpu, val);
        cpu->PC += 1;  
        break;
    }
}


