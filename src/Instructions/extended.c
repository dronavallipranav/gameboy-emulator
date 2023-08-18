#include "extended.h"
#include <stdint.h>
#include <cpu.h>
#include "bitmanip.h"
#include <stdlib.h>
#include <stdio.h>

void swap(Z80_State *cpu, uint8_t *bits)
{
    uint8_t low = *bits & 0x0F;
    uint8_t high = *bits & 0xF0;
    low = low << 4;
    high = high >> 4;
    *bits = low | high;
    cpu->AF.flags.Z = *bits == 0 ? 1 : 0;
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = 0;
    cpu->AF.flags.C = 0;
}

void resetBit(Z80_State* cpu, uint8_t bitPosition, uint8_t(*getReg)(Z80_State*), void(*setReg)(Z80_State*, uint8_t)) {
    uint8_t value = getReg(cpu);             
    uint8_t mask = ~(1 << bitPosition);    
    value &= mask;                         
    setReg(cpu, value);                     
}

void setBit(Z80_State* cpu, uint8_t bitPosition, uint8_t(*getReg)(Z80_State*), void(*setReg)(Z80_State*, uint8_t)) {
    uint8_t value = getReg(cpu);          
    value |= (1 << bitPosition);                       
    setReg(cpu, value);                  
}

void testBit(Z80_State* cpu, uint8_t bitPosition, uint8_t(*getReg)(Z80_State*)) {
    uint8_t value = getReg(cpu);
    uint8_t mask = (1 << bitPosition);
    uint8_t result = value & mask;

    cpu->AF.flags.Z = (result == 0);
    cpu->AF.flags.N = 0;             
    cpu->AF.flags.H = 1;             
}

void handler_cb(Z80_State *cpu, uint8_t opcode)
{
    uint8_t val;
    uint8_t temp_bit;
    switch (opcode)
    {
    // SWAP
    case 0x37:
        val = cpu->AF.A;
        swap(cpu, &val);
        cpu->AF.A = val;
        break;
    case 0x30:
        val = cpu->BC.B;
        swap(cpu, &val);
        cpu->BC.B = val;
        break;

    case 0x31:
        val = cpu->BC.C;
        swap(cpu, &val);
        cpu->BC.C = val;
        break;

    case 0x32:
        val = cpu->DE.D;
        swap(cpu, &val);
        cpu->DE.D = val;
        break;

    case 0x33:
        val = cpu->DE.E;
        swap(cpu, &val);
        cpu->DE.E = val;
        break;

    case 0x34:
        val = cpu->HL.H;
        swap(cpu, &val);
        cpu->HL.H = val;
        break;

    case 0x35:
        val = cpu->HL.L;
        swap(cpu, &val);
        cpu->HL.L = val;
        break;

    case 0x36:
        val = cpu->memory[cpu->HL_pair];
        swap(cpu, &val);
        cpu->memory[cpu->HL_pair] = val;
        break;

    // RLC
    case 0x07:
        rotate(cpu, true, true, cpu->getA, cpu->setA);
        break;

    case 0x00:
        rotate(cpu, true, true, cpu->getB, cpu->setB);
        break;

    case 0x01:
        rotate(cpu, true, true, cpu->getC, cpu->setC);
        break;

    case 0x02:
        rotate(cpu, true, true, cpu->getD, cpu->setD);
        break;

    case 0x03:
        rotate(cpu, true, true, cpu->getE, cpu->setE);
        break;

    case 0x04:
        rotate(cpu, true, true, cpu->getH, cpu->setH);
        break;

    case 0x05:
        rotate(cpu, true, true, cpu->getL, cpu->setL);
        break;

    // RLC (HL)
    case 0x06:
        val = cpu->memory[cpu->HL_pair];
        temp_bit = val & 0x80;
        val <<= 1;

        val |= (temp_bit >> 7);

        cpu->AF.flags.C = temp_bit != 0;

        cpu->AF.flags.Z = (val == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 0;

        cpu->memory[cpu->HL_pair] = val;
        break;

    // RL
    case 0x17:
        rotate(cpu, true, false, cpu->getA, cpu->setA);
        break;

    case 0x10:
        rotate(cpu, true, false, cpu->getB, cpu->setB);
        break;

    case 0x11:
        rotate(cpu, true, false, cpu->getC, cpu->setC);
        break;

    case 0x12:
        rotate(cpu, true, false, cpu->getD, cpu->setD);
        break;

    case 0x13:
        rotate(cpu, true, false, cpu->getE, cpu->setE);
        break;

    case 0x14:
        rotate(cpu, true, false, cpu->getH, cpu->setH);
        break;

    case 0x15:
        rotate(cpu, true, false, cpu->getL, cpu->setL);
        break;

    // RL (HL)
    case 0x16:
        val = cpu->memory[cpu->HL_pair];
        temp_bit = val & 0x80;
        val <<= 1;

        cpu->AF.flags.C = temp_bit != 0;

        cpu->AF.flags.Z = (val == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 0;

        cpu->memory[cpu->HL_pair] = val;
        break;

        // RRC
    case 0x0F:
        rotate(cpu, false, true, cpu->getA, cpu->setA);
        break;

    case 0x08:
        rotate(cpu, false, true, cpu->getB, cpu->setB);
        break;

    case 0x09:
        rotate(cpu, false, true, cpu->getC, cpu->setC);
        break;

    case 0x0A:
        rotate(cpu, false, true, cpu->getD, cpu->setD);
        break;

    case 0x0B:
        rotate(cpu, false, true, cpu->getE, cpu->setE);
        break;

    case 0x0C:
        rotate(cpu, false, true, cpu->getH, cpu->setH);
        break;

    case 0x0D:
        rotate(cpu, false, true, cpu->getL, cpu->setL);
        break;

    // RRC (HL)
    case 0x0E:
        val = cpu->memory[cpu->HL_pair];
        temp_bit = val & 0x01;
        val >>= 1;

        val |= (temp_bit << 7);

        cpu->AF.flags.C = temp_bit != 0;

        cpu->AF.flags.Z = (val == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 0;

        cpu->memory[cpu->HL_pair] = val;
        break;

    // RR
    case 0x1F:
        rotate(cpu, false, false, cpu->getA, cpu->setA);
        break;

    case 0x18:
        rotate(cpu, false, false, cpu->getB, cpu->setB);
        break;

    case 0x19:
        rotate(cpu, false, false, cpu->getC, cpu->setC);
        break;

    case 0x1A:
        rotate(cpu, false, false, cpu->getD, cpu->setD);
        break;

    case 0x1B:
        rotate(cpu, false, false, cpu->getE, cpu->setE);
        break;

    case 0x1C:
        rotate(cpu, false, false, cpu->getH, cpu->setH);
        break;

    case 0x1D:
        rotate(cpu, false, false, cpu->getL, cpu->setL);
        break;

    // RR (HL)
    case 0x1E:
        val = cpu->memory[cpu->HL_pair];
        temp_bit = val & 0x01;
        val >>= 1;

        cpu->AF.flags.C = temp_bit != 0;

        cpu->AF.flags.Z = (val == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 0;

        cpu->memory[cpu->HL_pair] = val;
        break;

    // SLA
    case 0x27:
        shift(cpu, true, false, cpu->getA, cpu->setA);
        break;

    case 0x20:
        shift(cpu, true, false, cpu->getB, cpu->setB);
        break;
    case 0x21:
        shift(cpu, true, false, cpu->getC, cpu->setC);
        break;
    case 0x22:
        shift(cpu, true, false, cpu->getD, cpu->setD);
        break;
    case 0x23:
        shift(cpu, true, false, cpu->getE, cpu->setE);
        break;
    case 0x24:
        shift(cpu, true, false, cpu->getH, cpu->setH);
        break;
    case 0x25:
        shift(cpu, true, false, cpu->getL, cpu->setL);
        break;
    case 0x26:
        val = cpu->memory[cpu->HL_pair];
        temp_bit = val & 0x80;
        val <<= 1;
        cpu->AF.flags.C = temp_bit != 0;

        cpu->AF.flags.Z = (val == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 0;

        cpu->memory[cpu->HL_pair] = val;
        break;

    // SRA
    case 0x2F:
        shift(cpu, false, true, cpu->getA, cpu->setA);
        break;
    case 0x28:
        shift(cpu, false, true, cpu->getB, cpu->setB);
        break;
    case 0x29:
        shift(cpu, false, true, cpu->getC, cpu->setC);
        break;
    case 0x2A:
        shift(cpu, false, true, cpu->getD, cpu->setD);
        break;
    case 0x2B:
        shift(cpu, false, true, cpu->getE, cpu->setE);
        break;
    case 0x2C:
        shift(cpu, false, true, cpu->getH, cpu->setH);
        break;
    case 0x2D:
        shift(cpu, false, true, cpu->getL, cpu->setL);
        break;
    case 0x2E:
        val = cpu->memory[cpu->HL_pair];
        temp_bit = val & 0x80;
        val >>= 1;
        val |= temp_bit;

        cpu->AF.flags.C = (val & 0x01) != 0;

        cpu->AF.flags.Z = (val == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 0;

        cpu->memory[cpu->HL_pair] = val;
        break;

    // SRL
    case 0x3F:
        shift(cpu, false, false, cpu->getA, cpu->setA);
        break;
    case 0x38:
        shift(cpu, false, false, cpu->getB, cpu->setB);
        break;
    case 0x39:
        shift(cpu, false, false, cpu->getC, cpu->setC);
        break;
    case 0x3A:
        shift(cpu, false, false, cpu->getD, cpu->setD);
        break;
    case 0x3B:
        shift(cpu, false, false, cpu->getE, cpu->setE);
        break;
    case 0x3C:
        shift(cpu, false, false, cpu->getH, cpu->setH);
        break;
    case 0x3D:
        shift(cpu, false, false, cpu->getL, cpu->setL);
        break;
    case 0x3E:
        val = cpu->memory[cpu->HL_pair];
        val >>= 1;

        cpu->AF.flags.C = (val & 0x01) != 0;
        cpu->AF.flags.Z = (val == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 0;

        cpu->memory[cpu->HL_pair] = val;
        break;

    //Reset
    
    
    case 0x80: resetBit(cpu, 0, cpu->getB, cpu->setB); break;
    case 0x81: resetBit(cpu, 0, cpu->getC, cpu->setC); break;
    case 0x82: resetBit(cpu, 0, cpu->getD, cpu->setD); break;
    case 0x83: resetBit(cpu, 0, cpu->getE, cpu->setE); break;
    case 0x84: resetBit(cpu, 0, cpu->getH, cpu->setH); break;
    case 0x85: resetBit(cpu, 0, cpu->getL, cpu->setL); break;
    case 0x86: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        temp_bit = (uint8_t) ~(1 <<0);
        val &= temp_bit;
        write_byte(cpu->mmu, cpu->HL_pair, val);  
    case 0x87: resetBit(cpu, 0, cpu->getA, cpu->setA); break;

   case 0x88: resetBit(cpu, 1, cpu->getB, cpu->setB); break;
    case 0x89: resetBit(cpu, 1, cpu->getC, cpu->setC); break;
    case 0x8A: resetBit(cpu, 1, cpu->getD, cpu->setD); break;
    case 0x8B: resetBit(cpu, 1, cpu->getE, cpu->setE); break;
    case 0x8C: resetBit(cpu, 1, cpu->getH, cpu->setH); break;
    case 0x8D: resetBit(cpu, 1, cpu->getL, cpu->setL); break;
    case 0x8E: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        temp_bit = (uint8_t) ~(1 << 1);
        val &= temp_bit;
        write_byte(cpu->mmu, cpu->HL_pair, val);  
    case 0x8F: resetBit(cpu, 1, cpu->getA, cpu->setA); break;

    
    case 0x90: resetBit(cpu, 2, cpu->getB, cpu->setB); break;
    case 0x91: resetBit(cpu, 2, cpu->getC, cpu->setC); break;
    case 0x92: resetBit(cpu, 2, cpu->getD, cpu->setD); break;
    case 0x93: resetBit(cpu, 2, cpu->getE, cpu->setE); break;
    case 0x94: resetBit(cpu, 2, cpu->getH, cpu->setH); break;
    case 0x95: resetBit(cpu, 2, cpu->getL, cpu->setL); break;
    case 0x96: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        temp_bit = (uint8_t) ~(1 <<2);
        val &= temp_bit;
        write_byte(cpu->mmu, cpu->HL_pair, val);  
    case 0x97: resetBit(cpu, 2, cpu->getA, cpu->setA); break;

    case 0x98: resetBit(cpu, 3, cpu->getB, cpu->setB); break;
    case 0x99: resetBit(cpu, 3, cpu->getC, cpu->setC); break;
    case 0x9A: resetBit(cpu, 3, cpu->getD, cpu->setD); break;
    case 0x9B: resetBit(cpu, 3, cpu->getE, cpu->setE); break;
    case 0x9C: resetBit(cpu, 3, cpu->getH, cpu->setH); break;
    case 0x9D: resetBit(cpu, 3, cpu->getL, cpu->setL); break;
    case 0x9E: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        temp_bit = (uint8_t) ~(1 << 3);
        val &= temp_bit;
        write_byte(cpu->mmu, cpu->HL_pair, val);  
    case 0x9F: resetBit(cpu, 3, cpu->getA, cpu->setA); break;

    case 0xA0: resetBit(cpu, 4, cpu->getB, cpu->setB); break;
    case 0xA1: resetBit(cpu, 4, cpu->getC, cpu->setC); break;
    case 0xA2: resetBit(cpu, 4, cpu->getD, cpu->setD); break;
    case 0xA3: resetBit(cpu, 4, cpu->getE, cpu->setE); break;
    case 0xA4: resetBit(cpu, 4, cpu->getH, cpu->setH); break;
    case 0xA5: resetBit(cpu, 4, cpu->getL, cpu->setL); break;
    case 0xA6: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        temp_bit = (uint8_t) ~(1 <<4);
        val &= temp_bit;
        write_byte(cpu->mmu, cpu->HL_pair, val);  
    case 0xA7: resetBit(cpu, 4, cpu->getA, cpu->setA); break;

    case 0xA8: resetBit(cpu, 5, cpu->getB, cpu->setB); break;
    case 0xA9: resetBit(cpu, 5, cpu->getC, cpu->setC); break;
    case 0xAA: resetBit(cpu, 5, cpu->getD, cpu->setD); break;
    case 0xAB: resetBit(cpu, 5, cpu->getE, cpu->setE); break;
    case 0xAC: resetBit(cpu, 5, cpu->getH, cpu->setH); break;
    case 0xAD: resetBit(cpu, 5, cpu->getL, cpu->setL); break;
    case 0xAE: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        temp_bit = (uint8_t) ~(1 << 5);
        val &= temp_bit;
        write_byte(cpu->mmu, cpu->HL_pair, val);  
    case 0xAF: resetBit(cpu, 5, cpu->getA, cpu->setA); break;

    case 0xB0: resetBit(cpu, 6, cpu->getB, cpu->setB); break;
    case 0xB1: resetBit(cpu, 6, cpu->getC, cpu->setC); break;
    case 0xB2: resetBit(cpu, 6, cpu->getD, cpu->setD); break;
    case 0xB3: resetBit(cpu, 6, cpu->getE, cpu->setE); break;
    case 0xB4: resetBit(cpu, 6, cpu->getH, cpu->setH); break;
    case 0xB5: resetBit(cpu, 6, cpu->getL, cpu->setL); break;
    case 0xB6: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        temp_bit = (uint8_t) ~(1 <<6);
        val &= temp_bit;
        write_byte(cpu->mmu, cpu->HL_pair, val);  
    case 0xB7: resetBit(cpu, 6, cpu->getA, cpu->setA); break;

    case 0xB8: resetBit(cpu, 7, cpu->getB, cpu->setB); break;
    case 0xB9: resetBit(cpu, 7, cpu->getC, cpu->setC); break;
    case 0xBA: resetBit(cpu, 7, cpu->getD, cpu->setD); break;
    case 0xBB: resetBit(cpu, 7, cpu->getE, cpu->setE); break;
    case 0xBC: resetBit(cpu, 7, cpu->getH, cpu->setH); break;
    case 0xBD: resetBit(cpu, 7, cpu->getL, cpu->setL); break;
    case 0xBE: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        temp_bit = (uint8_t) ~(1 << 7);
        val &= temp_bit;
        write_byte(cpu->mmu, cpu->HL_pair, val);  
    case 0xBF: resetBit(cpu, 7, cpu->getA, cpu->setA); break;

    //SET
    case 0xC0: setBit(cpu, 0, cpu->getB, cpu->setB); break;
    case 0xC1: setBit(cpu, 0, cpu->getC, cpu->setC); break;
    case 0xC2: setBit(cpu, 0, cpu->getD, cpu->setD); break;
    case 0xC3: setBit(cpu, 0, cpu->getE, cpu->setE); break;
    case 0xC4: setBit(cpu, 0, cpu->getH, cpu->setH); break;
    case 0xC5: setBit(cpu, 0, cpu->getL, cpu->setL); break;
    case 0xC6: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        val |= (1 << 0);
        write_byte(cpu->mmu, cpu->HL_pair, val);
        break;
    case 0xC7: setBit(cpu, 0, cpu->getA, cpu->setA); break;

    case 0xC8: setBit(cpu, 1, cpu->getB, cpu->setB); break;
    case 0xC9: setBit(cpu, 1, cpu->getC, cpu->setC); break;
    case 0xCA: setBit(cpu, 1, cpu->getD, cpu->setD); break;
    case 0xCB: setBit(cpu, 1, cpu->getE, cpu->setE); break;
    case 0xCC: setBit(cpu, 1, cpu->getH, cpu->setH); break;
    case 0xCD: setBit(cpu, 1, cpu->getL, cpu->setL); break;
    case 0xCE: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        val |= (1 << 1);
        write_byte(cpu->mmu, cpu->HL_pair, val);
        break;
    case 0xCF: setBit(cpu, 1, cpu->getA, cpu->setA); break;

    case 0xD0: setBit(cpu, 2, cpu->getB, cpu->setB); break;
    case 0xD1: setBit(cpu, 2, cpu->getC, cpu->setC); break;
    case 0xD2: setBit(cpu, 2, cpu->getD, cpu->setD); break;
    case 0xD3: setBit(cpu, 2, cpu->getE, cpu->setE); break;
    case 0xD4: setBit(cpu, 2, cpu->getH, cpu->setH); break;
    case 0xD5: setBit(cpu, 2, cpu->getL, cpu->setL); break;
    case 0xD6: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        val |= (1 << 2);
        write_byte(cpu->mmu, cpu->HL_pair, val);
        break;
    case 0xD7: setBit(cpu, 2, cpu->getA, cpu->setA); break;

    case 0xD8: setBit(cpu, 3, cpu->getB, cpu->setB); break;
    case 0xD9: setBit(cpu, 3, cpu->getC, cpu->setC); break;
    case 0xDA: setBit(cpu, 3, cpu->getD, cpu->setD); break;
    case 0xDB: setBit(cpu, 3, cpu->getE, cpu->setE); break;
    case 0xDC: setBit(cpu, 3, cpu->getH, cpu->setH); break;
    case 0xDD: setBit(cpu, 3, cpu->getL, cpu->setL); break;
    case 0xDE: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        val |= (1 << 3);
        write_byte(cpu->mmu, cpu->HL_pair, val);
        break;
    case 0xDF: setBit(cpu, 3, cpu->getA, cpu->setA); break;

    case 0xE0: setBit(cpu, 4, cpu->getB, cpu->setB); break;
    case 0xE1: setBit(cpu, 4, cpu->getC, cpu->setC); break;
    case 0xE2: setBit(cpu, 4, cpu->getD, cpu->setD); break;
    case 0xE3: setBit(cpu, 4, cpu->getE, cpu->setE); break;
    case 0xE4: setBit(cpu, 4, cpu->getH, cpu->setH); break;
    case 0xE5: setBit(cpu, 4, cpu->getL, cpu->setL); break;
    case 0xE6: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        val |= (1 << 4);
        write_byte(cpu->mmu, cpu->HL_pair, val);
        break;
    case 0xE7: setBit(cpu, 4, cpu->getA, cpu->setA); break;

    case 0xE8: setBit(cpu, 5, cpu->getB, cpu->setB); break;
    case 0xE9: setBit(cpu, 5, cpu->getC, cpu->setC); break;
    case 0xEA: setBit(cpu, 5, cpu->getD, cpu->setD); break;
    case 0xEB: setBit(cpu, 5, cpu->getE, cpu->setE); break;
    case 0xEC: setBit(cpu, 5, cpu->getH, cpu->setH); break;
    case 0xED: setBit(cpu, 5, cpu->getL, cpu->setL); break;
    case 0xEE: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        val |= (1 << 5);
        write_byte(cpu->mmu, cpu->HL_pair, val);
        break;
    case 0xEF: setBit(cpu, 5, cpu->getA, cpu->setA); break;

    case 0xF0: setBit(cpu, 6, cpu->getB, cpu->setB); break;
    case 0xF1: setBit(cpu, 6, cpu->getC, cpu->setC); break;
    case 0xF2: setBit(cpu, 6, cpu->getD, cpu->setD); break;
    case 0xF3: setBit(cpu, 6, cpu->getE, cpu->setE); break;
    case 0xF4: setBit(cpu, 6, cpu->getH, cpu->setH); break;
    case 0xF5: setBit(cpu, 6, cpu->getL, cpu->setL); break;
    case 0xF6: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        val |= (1 << 6);
        write_byte(cpu->mmu, cpu->HL_pair, val);
        break;
    case 0xF7: setBit(cpu, 6, cpu->getA, cpu->setA); break;

    case 0xF8: setBit(cpu, 7, cpu->getB, cpu->setB); break;
    case 0xF9: setBit(cpu, 7, cpu->getC, cpu->setC); break;
    case 0xFA: setBit(cpu, 7, cpu->getD, cpu->setD); break;
    case 0xFB: setBit(cpu, 7, cpu->getE, cpu->setE); break;
    case 0xFC: setBit(cpu, 7, cpu->getH, cpu->setH); break;
    case 0xFD: setBit(cpu, 7, cpu->getL, cpu->setL); break;
    case 0xFE: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        val |= (1 << 7);
        write_byte(cpu->mmu, cpu->HL_pair, val);
        break;
    case 0xFF: setBit(cpu, 7, cpu->getA, cpu->setA); break;


    //TEST
    case 0x40: testBit(cpu, 0, cpu->getB); break;
    case 0x41: testBit(cpu, 0, cpu->getC); break;
    case 0x42: testBit(cpu, 0, cpu->getD); break;
    case 0x43: testBit(cpu, 0, cpu->getE); break;
    case 0x44: testBit(cpu, 0, cpu->getH); break;
    case 0x45: testBit(cpu, 0, cpu->getL); break;
    case 0x46: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        cpu->AF.flags.Z = ((val & (1 << 0)) == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 1;
        break;
    case 0x47: testBit(cpu, 0, cpu->getA); break;

    case 0x48: testBit(cpu, 1, cpu->getB); break;
    case 0x49: testBit(cpu, 1, cpu->getC); break;
    case 0x4A: testBit(cpu, 1, cpu->getD); break;
    case 0x4B: testBit(cpu, 1, cpu->getE); break;
    case 0x4C: testBit(cpu, 1, cpu->getH); break;
    case 0x4D: testBit(cpu, 1, cpu->getL); break;
    case 0x4E: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        cpu->AF.flags.Z = ((val & (1 << 1)) == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 1;
        break;
    case 0x4F: testBit(cpu, 1, cpu->getA); break;

    case 0x50: testBit(cpu, 2, cpu->getB); break;
    case 0x51: testBit(cpu, 2, cpu->getC); break;
    case 0x52: testBit(cpu, 2, cpu->getD); break;
    case 0x53: testBit(cpu, 2, cpu->getE); break;
    case 0x54: testBit(cpu, 2, cpu->getH); break;
    case 0x55: testBit(cpu, 2, cpu->getL); break;
    case 0x56: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        cpu->AF.flags.Z = ((val & (1 << 2)) == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 1;
        break;
    case 0x57: testBit(cpu, 2, cpu->getA); break;

    case 0x58: testBit(cpu, 3, cpu->getB); break;
    case 0x59: testBit(cpu, 3, cpu->getC); break;
    case 0x5A: testBit(cpu, 3, cpu->getD); break;
    case 0x5B: testBit(cpu, 3, cpu->getE); break;
    case 0x5C: testBit(cpu, 3, cpu->getH); break;
    case 0x5D: testBit(cpu, 3, cpu->getL); break;
    case 0x5E: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        cpu->AF.flags.Z = ((val & (1 << 3)) == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 1;
        break;
    case 0x5F: testBit(cpu, 3, cpu->getA); break;

    case 0x60: testBit(cpu, 4, cpu->getB); break;
    case 0x61: testBit(cpu, 4, cpu->getC); break;
    case 0x62: testBit(cpu, 4, cpu->getD); break;
    case 0x63: testBit(cpu, 4, cpu->getE); break;
    case 0x64: testBit(cpu, 4, cpu->getH); break;
    case 0x65: testBit(cpu, 4, cpu->getL); break;
    case 0x66: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        cpu->AF.flags.Z = ((val & (1 << 4)) == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 1;
        break;
    case 0x67: testBit(cpu, 4, cpu->getA); break;

    case 0x68: testBit(cpu, 5, cpu->getB); break;
    case 0x69: testBit(cpu, 5, cpu->getC); break;
    case 0x6A: testBit(cpu, 5, cpu->getD); break;
    case 0x6B: testBit(cpu, 5, cpu->getE); break;
    case 0x6C: testBit(cpu, 5, cpu->getH); break;
    case 0x6D: testBit(cpu, 5, cpu->getL); break;
    case 0x6E: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        cpu->AF.flags.Z = ((val & (1 << 5)) == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 1;
        break;
    case 0x6F: testBit(cpu, 5, cpu->getA); break;

    case 0x70: testBit(cpu, 6, cpu->getB); break;
    case 0x71: testBit(cpu, 6, cpu->getC); break;
    case 0x72: testBit(cpu, 6, cpu->getD); break;
    case 0x73: testBit(cpu, 6, cpu->getE); break;
    case 0x74: testBit(cpu, 6, cpu->getH); break;
    case 0x75: testBit(cpu, 6, cpu->getL); break;
    case 0x76: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        cpu->AF.flags.Z = ((val & (1 << 6)) == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 1;
        break;
    case 0x77: testBit(cpu, 6, cpu->getA); break;

    case 0x78: testBit(cpu, 7, cpu->getB); break;
    case 0x79: testBit(cpu, 7, cpu->getC); break;
    case 0x7A: testBit(cpu, 7, cpu->getD); break;
    case 0x7B: testBit(cpu, 7, cpu->getE); break;
    case 0x7C: testBit(cpu, 7, cpu->getH); break;
    case 0x7D: testBit(cpu, 7, cpu->getL); break;
    case 0x7E: 
        val = read_byte(cpu->mmu, cpu->HL_pair);
        cpu->AF.flags.Z = ((val & (1 << 7)) == 0);
        cpu->AF.flags.N = 0;
        cpu->AF.flags.H = 1;
        break;
    case 0x7F: testBit(cpu, 7, cpu->getA); break;


    default:
            fprintf(stderr, "Unhandled CB opcode: 0x%02X at PC: 0x%04X\n", opcode, cpu->PC);
            exit(1);
        
    }
}