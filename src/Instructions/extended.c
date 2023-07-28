#include <extended.h>
#include <stdint.h>
#include <cpu.h>

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

void handler_cp(Z80_State *cpu, uint8_t opcode)
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
    }
}