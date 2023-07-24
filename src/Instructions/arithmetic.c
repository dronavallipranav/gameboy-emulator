#include <cpu.h>
#include <arithmetic.h>
#include <stdbool.h>

void add8(Z80_State *cpu, uint8_t reg, bool carryStatus){
    uint8_t carry = carryStatus ? cpu->AF.flags.C : 0;
    uint16_t result = cpu->AF.A + reg + carry;

    cpu->AF.flags.Z = ((result & 0xFF) == 0);
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = ((cpu->AF.A & 0x0F) + (reg & 0x0F) + carry) > 0x0F;
    cpu->AF.flags.C = result > 0xFF;
    cpu->AF.A = result & 0xFF; 
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
    
    switch(opcode){
        case 0x87:
        val = cpu->AF.A;
        add8(cpu, val, false);
        break;
    case 0x80:
        val = cpu->BC.B;
        add8(cpu, val, false);
        break;
    case 0x81:
        val = cpu->BC.C;
        add8(cpu, val, false);
        break;
    case 0x82:
        val = cpu->DE.D;
        add8(cpu, val, false);
        break;
    case 0x83:
        val = cpu->DE.E;
        add8(cpu, val, false);
        break;
    case 0x84:
        val = cpu->HL.H;
        add8(cpu, val, false);
        break;
    case 0x85:
        val = cpu->HL.L;
        add8(cpu, val, false);
        break;
    case 0x86:
        val = cpu->memory[cpu->HL_pair];
        add8(cpu, val, false);
        break;
    case 0xC6:
        val = cpu->memory[cpu->PC+1];
        add8(cpu, val, false);
        cpu->PC += 1;  
        break;

    //ADC operations
    case 0x8F:  
            val = cpu->AF.A;
            add8(cpu, val, true);
            break;
        case 0x88:  
            val = cpu->BC.B;
            add8(cpu, val, true);
            break;
        case 0x89:  
            val = cpu->BC.C;
            add8(cpu, val, true);
            break;
        case 0x8A:
            val = cpu->DE.D;
            add8(cpu, val, true);
            break;
        case 0x8B:  
            val = cpu->DE.E;
            add8(cpu, val, true);
            break;
        case 0x8C: 
            val = cpu->HL.H;
            add8(cpu, val, true);
            break;
        case 0x8D: 
            val = cpu->HL.L;
            add8(cpu, val, true);
            break;
        case 0x8E: 
            val = cpu->memory[cpu->HL_pair];
            add8(cpu, val, true);
            break;
        case 0xCE: 
            val = cpu->memory[cpu->PC+1];
            add8(cpu, val, true);
            cpu->PC += 1;
            break;
    }
}


