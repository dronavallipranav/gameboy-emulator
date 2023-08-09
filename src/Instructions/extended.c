#include <extended.h>
#include <stdint.h>
#include <cpu.h>
#include <bitmanip.h>
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

void handler_cb(Z80_State *cpu, uint8_t opcode)
{
    uint8_t val;
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
        uint8_t high_bit = val & 0x80;
        val <<= 1;

        val |= (high_bit >> 7);

        cpu->AF.flags.C = high_bit != 0;

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
        uint8_t high_bit = val & 0x80;
        val <<= 1;

        cpu->AF.flags.C = high_bit != 0;

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
        uint8_t low_bit = val & 0x01;
        val >>= 1;

        val |= (low_bit << 7);

        cpu->AF.flags.C = low_bit != 0;

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
        uint8_t low_bit = val & 0x01;
        val >>= 1;

        cpu->AF.flags.C = low_bit != 0;

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
        uint8_t msb = val & 0x80;
        val <<= 1;
        cpu->AF.flags.C = msb != 0;

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
        uint8_t msb = val & 0x80;
        val >>= 1;
        val |= msb;

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
    default:
            fprintf(stderr, "Unhandled CB opcode: 0x%02X at PC: 0x%04X\n", opcode, cpu->PC);
            exit(1);
        
    }
}