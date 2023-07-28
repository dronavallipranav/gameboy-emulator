#include <cpu.h>
#include <arithmetic.h>
#include <stdbool.h>

void add8(Z80_State *cpu, uint8_t reg, bool carryStatus)
{
    uint16_t result = cpu->AF.A + reg + (carryStatus ? cpu->AF.flags.C : 0);

    cpu->AF.flags.Z = ((result & 0xFF) == 0);
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = ((cpu->AF.A & 0x0F) + (reg & 0x0F) + (carryStatus ? cpu->AF.flags.C : 0)) > 0x0F;
    cpu->AF.flags.C = result > 0xFF;
    cpu->AF.A = result & 0xFF;
}

void add16(Z80_State *cpu,  void(*setReg)(Z80_State*, uint16_t), uint16_t(*getReg)(Z80_State*) ,uint16_t reg)
{
    uint32_t val = (uint32_t) getReg(cpu);
    val += (uint32_t) reg;

    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = ((getReg(cpu) & 0x0FFF) + (reg & 0x0FFF)) > 0x0FFF;
    cpu->AF.flags.C = val > 0xFFFF;

    setReg(cpu, val & 0xFFFF);
}

void addImm(Z80_State *cpu, uint16_t imm)
{
    cpu->AF.A += imm;
}

void sub8(Z80_State *cpu, uint8_t reg, bool carryStatus)
{
    uint16_t result = cpu->AF.A - reg - (carryStatus ? cpu->AF.flags.C : 0);

    cpu->AF.flags.Z = (result & 0xFF) == 0;
    cpu->AF.flags.N = 1;
    cpu->AF.flags.H = (cpu->AF.A & 0x0F) < (reg & 0x0F);
    cpu->AF.flags.C = cpu->AF.A < (reg + (carryStatus ? cpu->AF.flags.C : 0));
    cpu->AF.A = result & 0xFF;
}

void sub16(Z80_State *cpu, uint16_t reg)
{
    cpu->AF.A -= reg;
}

void subImm(Z80_State *cpu, uint16_t imm)
{
    cpu->AF.A -= imm;
}

void and8(Z80_State *cpu, uint8_t reg)
{
    uint8_t res = cpu->AF.A & reg;
    cpu->AF.flags.Z = (res & 0xFF) == 0;
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = 1;
    cpu->AF.flags.C = 0;
    cpu->AF.A = res;
}

void or8(Z80_State *cpu, uint8_t reg)
{
    uint8_t res = cpu->AF.A | reg;
    cpu->AF.flags.Z = (res & 0xFF) == 0;
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = 0;
    cpu->AF.flags.C = 0;
    cpu->AF.A = res;
}

void xor8(Z80_State *cpu, uint8_t reg)
{
    uint8_t res = cpu->AF.A ^ reg;
    cpu->AF.flags.Z = (res & 0xFF) == 0;
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = 0;
    cpu->AF.flags.C = 0;
    cpu->AF.A = res;
}

void cp8(Z80_State *cpu, uint8_t reg)
{
    cpu->AF.flags.Z = reg > cpu->AF.A;
    cpu->AF.flags.N = 1;
    cpu->AF.flags.H = (cpu->AF.A & 0x0F) < (reg & 0x0F);
    cpu->AF.flags.C = cpu->AF.A < reg;
}

void inc8(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t val)
{
    cpu->AF.flags.Z = val + 1 == 0;
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = val + 1 > 0x0F;
    setReg(cpu, val + 1);
}

void dec8(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t val)
{
    cpu->AF.flags.Z = val - 1 == 0;
    cpu->AF.flags.N = 1;
    cpu->AF.flags.H = (val & 0x0F) == 0;
    setReg(cpu, val - 1);
}

void ALU(Z80_State *cpu, uint8_t opcode)
{
    uint8_t val;
    uint16_t addr;
    void(*setReg16)(Z80_State *, uint16_t);
    uint16_t (*getReg16)(Z80_State*);

    switch (opcode)
    {
    //ADD ops
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
        val = cpu->memory[cpu->PC + 1];
        add8(cpu, val, false);
        cpu->PC += 1;
        break;

    // ADC ops
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
        val = cpu->memory[cpu->PC + 1];
        add8(cpu, val, true);
        cpu->PC += 1;
        break;

    //SUB ops
    case 0x97:
        sub8(cpu, cpu->AF.A, false);
        break;
    case 0x90:
        sub8(cpu, cpu->BC.B, false);
        break;
    case 0x91:
        sub8(cpu, cpu->BC.C, false);
        break;
    case 0x92:
        sub8(cpu, cpu->DE.D, false);
        break;
    case 0x93:
        sub8(cpu, cpu->DE.E, false);
        break;
    case 0x94:
        sub8(cpu, cpu->HL.H, false);
        break;
    case 0x95:
        sub8(cpu, cpu->HL.L, false);
        break;
    case 0x96:
        addr = cpu->HL_pair;
        sub8(cpu, cpu->memory[addr], false);
        break;
    case 0xD6:
        addr = cpu->PC + 1;
        sub8(cpu, cpu->memory[addr], false);
        cpu->PC++;
        break;

    // SUBC
    case 0x9F:
        sub8(cpu, cpu->AF.A, true);
        break;
    case 0x98:
        sub8(cpu, cpu->BC.B, true);
        break;
    case 0x99:
        sub8(cpu, cpu->BC.C, true);
        break;
    case 0x9A:
        sub8(cpu, cpu->DE.D, true);
        break;
    case 0x9B:
        sub8(cpu, cpu->DE.E, true);
        break;
    case 0x9C:
        sub8(cpu, cpu->HL.H, true);
        break;
    case 0x9D:
        sub8(cpu, cpu->HL.L, true);
        break;
    case 0x9E:
        addr = cpu->HL_pair;
        sub8(cpu, cpu->memory[addr], true);
        break;

    // Logical Group AND
    case 0xA7:
        val = cpu->AF.A;
        and8(cpu, val);
        break;
    case 0xA0:
        val = cpu->BC.B;
        and8(cpu, val);
        break;
    case 0xA1:
        val = cpu->BC.C;
        and8(cpu, val);
        break;
    case 0xA2:
        val = cpu->DE.D;
        and8(cpu, val);
        break;
    case 0xA3:
        val = cpu->DE.E;
        and8(cpu, val);
        break;
    case 0xA4:
        val = cpu->HL.H;
        and8(cpu, val);
        break;
    case 0xA5:
        val = cpu->HL.L;
        and8(cpu, val);
        break;
    case 0xA6:
        val = cpu->memory[cpu->HL_pair];
        and8(cpu, val);
        break;
    case 0xE6:
        val = cpu->memory[cpu->PC + 1];
        cpu->PC += 1;
        and8(cpu, val);
        break;

    // OR ops
    case 0xB7:
        val = cpu->AF.A;
        or8(cpu, val);
        break;
    case 0xB0:
        val = cpu->BC.B;
        or8(cpu, val);
        break;
    case 0xB1:
        val = cpu->BC.C;
        or8(cpu, val);
        break;
    case 0xB2:
        val = cpu->DE.D;
        or8(cpu, val);
        break;
    case 0xB3:
        val = cpu->DE.E;
        or8(cpu, val);
        break;
    case 0xB4:
        val = cpu->HL.H;
        or8(cpu, val);
        break;
    case 0xB5:
        val = cpu->HL.L;
        or8(cpu, val);
        break;
    case 0xB6:
        val = cpu->memory[cpu->HL_pair];
        or8(cpu, val);
        break;
    case 0xF6:
        val = cpu->memory[cpu->PC + 1];
        cpu->PC += 1;
        or8(cpu, val);
        break;

    // XOR ops
    case 0xAF:
        val = cpu->AF.A;
        xor8(cpu, val);
        break;
    case 0xA8:
        val = cpu->BC.B;
        xor8(cpu, val);
        break;
    case 0xA9:
        val = cpu->BC.C;
        xor8(cpu, val);
        break;
    case 0xAA:
        val = cpu->DE.D;
        xor8(cpu, val);
        break;
    case 0xAB:
        val = cpu->DE.E;
        xor8(cpu, val);
        break;
    case 0xAC:
        val = cpu->HL.H;
        xor8(cpu, val);
        break;
    case 0xAD:
        val = cpu->HL.L;
        xor8(cpu, val);
        break;
    case 0xAE:
        val = cpu->memory[cpu->HL_pair];
        xor8(cpu, val);
        break;
    case 0xEE:
        val = cpu->memory[cpu->PC + 1];
        cpu->PC += 1;
        xor8(cpu, val);
        break;

    // CP ops
    case 0xBF:
        val = cpu->AF.A;
        cp8(cpu, val);
        break;
    case 0xB8:
        val = cpu->BC.B;
        cp8(cpu, val);
        break;
    case 0xB9:
        val = cpu->BC.C;
        cp8(cpu, val);
        break;
    case 0xBA:
        val = cpu->DE.D;
        cp8(cpu, val);
        break;
    case 0xBB:
        val = cpu->DE.E;
        cp8(cpu, val);
        break;
    case 0xBC:
        val = cpu->HL.H;
        cp8(cpu, val);
        break;
    case 0xBD:
        val = cpu->HL.L;
        cp8(cpu, val);
        break;
    case 0xBE:
        val = cpu->memory[cpu->HL_pair];
        cp8(cpu, val);
        break;
    case 0xFE:
        val = cpu->memory[cpu->PC + 1];
        cpu->PC += 1;
        cp8(cpu, val);
        break;

    // INC ops
    case 0x3C:
        inc8(cpu, cpu->setA, cpu->AF.A);
        break;
    case 0x04:
        inc8(cpu, cpu->setB, cpu->BC.B);
        break;
    case 0x0C:
        inc8(cpu, cpu->setC, cpu->BC.C);
        break;
    case 0x14:
        inc8(cpu, cpu->setD, cpu->DE.D);
        break;
    case 0x1C:
        inc8(cpu, cpu->setE, cpu->DE.E);
        break;
    case 0x24:
        inc8(cpu, cpu->setH, cpu->HL.H);
        break;
    case 0x2C:
        inc8(cpu, cpu->setL, cpu->HL.L);
        break;
    case 0x34:
        val = cpu->memory[cpu->HL_pair];
        cpu->AF.flags.Z = val + 1 == 0;
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = val + 1 > 0x0F;
        cpu->memory[cpu->HL_pair] = val + 1;
        break;

    // DEC ops
    case 0x3D:
        dec8(cpu, cpu->setA, cpu->AF.A);
        break;
    case 0x05:
        dec8(cpu, cpu->setB, cpu->BC.B);
        break;
    case 0x0D:
        dec8(cpu, cpu->setC, cpu->BC.C);
        break;
    case 0x15:
        dec8(cpu, cpu->setD, cpu->DE.D);
        break;
    case 0x1D:
        dec8(cpu, cpu->setE, cpu->DE.E);
        break;
    case 0x25:
        dec8(cpu, cpu->setH, cpu->HL.H);
        break;
    case 0x2D:
        dec8(cpu, cpu->setL, cpu->HL.L);
        break;
    case 0x35:
        val = cpu->memory[cpu->HL_pair];
        cpu->AF.flags.Z = (val - 1) == 0;
        cpu->AF.flags.N = 1;
        cpu->AF.flags.H = (val & 0x0F) == 0;
        cpu->memory[cpu->HL_pair] = val - 1;
        break;

    //16-bit arithmetic
    case 0x09:
        setReg16 = cpu -> setHL;
        getReg16 = cpu -> getHL;
        addr = cpu->BC_pair;
        add16(cpu, setReg16, getReg16, addr);
    
    case 0x19:  
        setReg16 = cpu->setHL;
        getReg16 = cpu -> getHL;
        addr = cpu->DE_pair;
        add16(cpu, setReg16, getReg16, addr);
        break;

    case 0x29: 
        setReg16 = cpu->setHL;
        getReg16 = cpu -> getHL;
        addr = cpu->HL_pair;
        add16(cpu, setReg16, getReg16, addr);
        break;

    case 0x39: 
        setReg16 = cpu->setHL;
        getReg16 = cpu -> getHL;
        addr = cpu->SP;
        add16(cpu, setReg16, getReg16, addr);
        break;

    case 0xE8: 
        val = cpu->memory[cpu->PC+1];
        uint32_t result = (uint32_t) cpu->SP + (uint32_t) val;
        cpu->AF.flags.Z = 0;
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = ((cpu->SP & 0x0FFF) + (val & 0x0FFF)) > 0x0FFF;
        cpu->AF.flags.C = result > 0xFFFF;
        cpu->SP = (uint16_t) (result & 0xFFFF);
        break;

//INC
case 0x03: 
    setReg16 = cpu->setBC;
    getReg16 = cpu->getBC;
    add16(cpu, setReg16, getReg16, 1);
    break;

case 0x13: 
    setReg16 = cpu->setDE;
    getReg16 = cpu->getDE;
    add16(cpu, setReg16, getReg16, 1);
    break;

case 0x23: 
    setReg16 = cpu->setHL;
    getReg16 = cpu->getHL;
    add16(cpu, setReg16, getReg16, 1);
    break;

case 0x33: 
    setReg16 = cpu->setSP;
    getReg16 = cpu->getSP;
    add16(cpu, setReg16, getReg16, 1);
    break;

//DEC
case 0x0B: 
    setReg16 = cpu->setBC;
    getReg16 = cpu->getBC;
    add16(cpu, setReg16, getReg16, -1);
    break;

case 0x1B: 
    setReg16 = cpu->setDE;
    getReg16 = cpu->getDE;
    add16(cpu, setReg16, getReg16, -1);
    break;

case 0x2B: 
    setReg16 = cpu->setHL;
    getReg16 = cpu->getHL;
    add16(cpu, setReg16, getReg16, -1);
    break;

case 0x3B: 
    setReg16 = cpu->setSP;
    getReg16 = cpu->getSP;
    add16(cpu, setReg16, getReg16, -1);
    break;

    
    }
}
