#include <cpu.h>
#include "bitmanip.h"
#include <stdio.h>

void daa(Z80_State *cpu)
{
    uint8_t correction = cpu->AF.flags.C ? 0x60 : 0x00;
    if (cpu->AF.flags.H || (!cpu->AF.flags.N && (cpu->AF.A & 0x0F) > 9))
    {
        correction |= 0x06;
    }
    if (cpu->AF.flags.C || (!cpu->AF.flags.N && cpu->AF.A > 0x99))
    {
        correction |= 0x60;
    }
    cpu->AF.A = cpu->AF.flags.N ? cpu->AF.A - correction : cpu->AF.A + correction;
    cpu->AF.flags.C = (correction >= 0x60) ? 1 : 0;
    cpu->AF.flags.H = 0;
    cpu->AF.flags.Z = (cpu->AF.A == 0) ? 1 : 0;
}

void cpl(Z80_State *cpu)
{
    cpu->AF.A = ~(cpu->AF.A);
    cpu->AF.flags.Z = cpu->AF.A == 0 ? 1 : 0;
    cpu->AF.flags.N = 1;
    cpu->AF.flags.H = 1;
}

void ccf(Z80_State *cpu)
{
    if (cpu->AF.flags.C == 1)
        cpu->AF.flags.C = 0;
    else
        cpu->AF.flags.C = 1;

    cpu->AF.flags.H = 0;
    cpu->AF.flags.N = 0;
}

void scf(Z80_State *cpu)
{
    cpu->AF.flags.C = 1;
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = 0;
}

void rotate(Z80_State *cpu, bool left, bool circular, uint8_t (*getReg)(Z80_State *), void (*setReg)(Z80_State *, uint8_t))
{
    uint8_t val = getReg(cpu); // Get register value
    // left rotate case
    if (left)
    {
        uint8_t high_bit = val & 0x80;
        val <<= 1;
        // circular left shift case, moves msb to lsb
        if (circular)
        {
            val |= (high_bit >> 7);
        }

        cpu->AF.flags.C = high_bit != 0;
    }
    else
    {
        uint8_t low_bit = val & 0x01;
        val >>= 1;
        // circular right shift case, moves lsb to msb
        if (circular)
        {
            val |= (low_bit << 7);
        }

        cpu->AF.flags.C = low_bit != 0;
    }

    // Set flags
    cpu->AF.flags.Z = (val == 0);
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = 0;

    setReg(cpu, val);
}

void shift(Z80_State *cpu, bool left, bool keep_msb, uint8_t (*getReg)(Z80_State *), void (*setReg)(Z80_State *, uint8_t))
{
    uint8_t val = getReg(cpu);
    uint8_t msb = val & 0x80;
    uint8_t bit;

    if (left)
    {
        bit = val & 0x80;
        val <<= 1;
    }
    else
    {
        bit = val & 0x01;
        val >>= 1;

        if (keep_msb)
        {
            val |= msb;
        }
    }

    cpu->AF.flags.C = bit != 0;
    cpu->AF.flags.Z = (val == 0);
    cpu->AF.flags.N = 0;
    cpu->AF.flags.H = 0;

    setReg(cpu, val);
}

void handle_manip(Z80_State *cpu, uint8_t opcode)
{
    switch (opcode)
    {
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
    // NOP
    case 0x00:
        break;

    // HALT
    case 0x76:
        cpu->Halt = true;
        break;

    // STOP
    case 0x10:
        cpu->Stop = true;
        break;
    // DI
    case 0xF3:
        cpu->DI_status = true;
        break;
    // EI
    case 0xFB:
        cpu->EI_status = true;
        break;

    case 0x07:
        rotate(cpu, true, true, cpu->getA, cpu->setA);
        break;

    case 0x17:
        rotate(cpu, true, false, cpu->getA, cpu->setA);
        break;

    case 0x0F:
        rotate(cpu, false, true, cpu->getA, cpu->setA);
        break;

    case 0x1F:
        rotate(cpu, false, false, cpu->getA, cpu->setA);
        break;

    default:
        fprintf(stderr, "Unhandled CB opcode: 0x%02X at PC: 0x%04X\n", opcode, cpu->PC);
        exit(1);
    }
}