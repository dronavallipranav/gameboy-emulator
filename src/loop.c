#include <cpu.h>
#include "./Instructions/bitmanip.h"
#include <stdio.h>
#include <arithmetic.h>
#include <movement.h>
#include <extended.h>

typedef void (*opcode_handler)(Z80_State *cpu, uint8_t opcode);

opcode_handler opcode_table[256];

void init_opcode_table()
{
    opcode_table[0x27] = handle_manip;
    opcode_table[0x2F] = handle_manip;
    opcode_table[0x3F] = handle_manip;
    opcode_table[0x37] = handle_manip;
    opcode_table[0x00] = handle_manip;
    opcode_table[0x76] = handle_manip;
    opcode_table[0x10] = handle_manip;
    opcode_table[0xF3] = handle_manip;
    opcode_table[0xFB] = handle_manip;
    opcode_table[0x07] = handle_manip;
    opcode_table[0x17] = handle_manip;
    opcode_table[0x0F] = handle_manip;
    opcode_table[0x1F] = handle_manip;
    opcode_table[0x87] = ALU;
    opcode_table[0x80] = ALU;
    opcode_table[0x81] = ALU;
    opcode_table[0x82] = ALU;
    opcode_table[0x83] = ALU;
    opcode_table[0x84] = ALU;
    opcode_table[0x85] = ALU;
    opcode_table[0x86] = ALU;
    opcode_table[0xC6] = ALU;
    opcode_table[0x8F] = ALU;
    opcode_table[0x88] = ALU;
    opcode_table[0x89] = ALU;
    opcode_table[0x8A] = ALU;
    opcode_table[0x8B] = ALU;
    opcode_table[0x8C] = ALU;
    opcode_table[0x8D] = ALU;
    opcode_table[0x8E] = ALU;
    opcode_table[0xCE] = ALU;
    opcode_table[0x97] = ALU;
    opcode_table[0x90] = ALU;
    opcode_table[0x91] = ALU;
    opcode_table[0x92] = ALU;
    opcode_table[0x93] = ALU;
    opcode_table[0x94] = ALU;
    opcode_table[0x95] = ALU;
    opcode_table[0x96] = ALU;
    opcode_table[0xD6] = ALU;
    opcode_table[0x9F] = ALU;
    opcode_table[0x98] = ALU;
    opcode_table[0x99] = ALU;
    opcode_table[0x9A] = ALU;
    opcode_table[0x9B] = ALU;
    opcode_table[0x9C] = ALU;
    opcode_table[0x9D] = ALU;
    opcode_table[0x9E] = ALU;
    opcode_table[0xA7] = ALU;
    opcode_table[0xA0] = ALU;
    opcode_table[0xA1] = ALU;
    opcode_table[0xA2] = ALU;
    opcode_table[0xA3] = ALU;
    opcode_table[0xA4] = ALU;
    opcode_table[0xA5] = ALU;
    opcode_table[0xA6] = ALU;
    opcode_table[0xE6] = ALU;
    opcode_table[0xB7] = ALU;
    opcode_table[0xB0] = ALU;
    opcode_table[0xB1] = ALU;
    opcode_table[0xB2] = ALU;
    opcode_table[0xB3] = ALU;
    opcode_table[0xB4] = ALU;
    opcode_table[0xB5] = ALU;
    opcode_table[0xB6] = ALU;
    opcode_table[0xF6] = ALU;
    opcode_table[0xAF] = ALU;
    opcode_table[0xA8] = ALU;
    opcode_table[0xA9] = ALU;
    opcode_table[0xAA] = ALU;
    opcode_table[0xAB] = ALU;
    opcode_table[0xAC] = ALU;
    opcode_table[0xAD] = ALU;
    opcode_table[0xAE] = ALU;
    opcode_table[0xEE] = ALU;
    opcode_table[0xBF] = ALU;
    opcode_table[0xB8] = ALU;
    opcode_table[0xB9] = ALU;
    opcode_table[0xBA] = ALU;
    opcode_table[0xBB] = ALU;
    opcode_table[0xBC] = ALU;
    opcode_table[0xBD] = ALU;
    opcode_table[0xBE] = ALU;
    opcode_table[0xFE] = ALU;
    opcode_table[0x3C] = ALU;
    opcode_table[0x04] = ALU;
    opcode_table[0x0C] = ALU;
    opcode_table[0x14] = ALU;
    opcode_table[0x1C] = ALU;
    opcode_table[0x24] = ALU;
    opcode_table[0x2C] = ALU;
    opcode_table[0x34] = ALU;
    opcode_table[0x3D] = ALU;
    opcode_table[0x05] = ALU;
    opcode_table[0x0D] = ALU;
    opcode_table[0x15] = ALU;
    opcode_table[0x1D] = ALU;
    opcode_table[0x25] = ALU;
    opcode_table[0x2D] = ALU;
    opcode_table[0x35] = ALU;
    opcode_table[0x09] = ALU;
    opcode_table[0x19] = ALU;
    opcode_table[0x29] = ALU;
    opcode_table[0x39] = ALU;
    opcode_table[0xE8] = ALU;
    opcode_table[0x03] = ALU;
    opcode_table[0x13] = ALU;
    opcode_table[0x23] = ALU;
    opcode_table[0x33] = ALU;
    opcode_table[0x0B] = ALU;
    opcode_table[0x1B] = ALU;
    opcode_table[0x2B] = ALU;
    opcode_table[0x3B] = ALU;
    opcode_table[0x01] = loadReg;
    opcode_table[0x11] = loadReg;
    opcode_table[0x21] = loadReg;
    opcode_table[0x31] = loadReg;
    opcode_table[0xF9] = loadReg;
    opcode_table[0xF8] = loadReg;
    opcode_table[0x08] = loadReg;
    opcode_table[0xF5] = loadReg;
    opcode_table[0xC5] = loadReg;
    opcode_table[0xD5] = loadReg;
    opcode_table[0xE5] = loadReg;
    opcode_table[0xF1] = loadReg;
    opcode_table[0xC1] = loadReg;
    opcode_table[0xD1] = loadReg;
    opcode_table[0xE1] = loadReg;
    opcode_table[0x02] = loadReg;
    opcode_table[0x12] = loadReg;
    opcode_table[0x77] = loadReg;
    opcode_table[0xEA] = loadReg;
    opcode_table[0x3A] = loadReg;
    opcode_table[0x32] = loadReg;
    opcode_table[0x2A] = loadReg;
    opcode_table[0x22] = loadReg;
    opcode_table[0xE0] = loadReg;
    opcode_table[0xF0] = loadReg;
    opcode_table[0x7F] = loadReg;
    opcode_table[0x78] = loadReg;
    opcode_table[0x79] = loadReg;
    opcode_table[0x7A] = loadReg;
    opcode_table[0x7B] = loadReg;
    opcode_table[0x7C] = loadReg;
    opcode_table[0x7D] = loadReg;
    opcode_table[0x7E] = loadReg;
    opcode_table[0x0A] = loadReg;
    opcode_table[0x1A] = loadReg;
    opcode_table[0xFA] = loadReg;
    opcode_table[0x3E] = loadReg;
    opcode_table[0xF2] = loadReg;
    opcode_table[0xE2] = loadReg;
    opcode_table[0x40] = loadReg;
    opcode_table[0x41] = loadReg;
    opcode_table[0x42] = loadReg;
    opcode_table[0x43] = loadReg;
    opcode_table[0x44] = loadReg;
    opcode_table[0x45] = loadReg;
    opcode_table[0x46] = loadReg;
    opcode_table[0x47] = loadReg;
    opcode_table[0x48] = loadReg;
    opcode_table[0x49] = loadReg;
    opcode_table[0x4A] = loadReg;
    opcode_table[0x4B] = loadReg;
    opcode_table[0x4C] = loadReg;
    opcode_table[0x4D] = loadReg;
    opcode_table[0x4E] = loadReg;
    opcode_table[0x4F] = loadReg;
    opcode_table[0x50] = loadReg;
    opcode_table[0x51] = loadReg;
    opcode_table[0x52] = loadReg;
    opcode_table[0x53] = loadReg;
    opcode_table[0x54] = loadReg;
    opcode_table[0x55] = loadReg;
    opcode_table[0x56] = loadReg;
    opcode_table[0x57] = loadReg;
    opcode_table[0x58] = loadReg;
    opcode_table[0x59] = loadReg;
    opcode_table[0x5A] = loadReg;
    opcode_table[0x5B] = loadReg;
    opcode_table[0x5C] = loadReg;
    opcode_table[0x5D] = loadReg;
    opcode_table[0x5E] = loadReg;
    opcode_table[0x60] = loadReg;
    opcode_table[0x61] = loadReg;
    opcode_table[0x62] = loadReg;
    opcode_table[0x63] = loadReg;
    opcode_table[0x64] = loadReg;
    opcode_table[0x65] = loadReg;
    opcode_table[0x66] = loadReg;
    opcode_table[0x67] = loadReg;
    opcode_table[0x68] = loadReg;
    opcode_table[0x69] = loadReg;
    opcode_table[0x6A] = loadReg;
    opcode_table[0x6B] = loadReg;
    opcode_table[0x6C] = loadReg;
    opcode_table[0x6D] = loadReg;
    opcode_table[0x6E] = loadReg;
    opcode_table[0x6F] = loadReg;
    opcode_table[0xC3] = loadReg;
    opcode_table[0x0E] = loadReg;
    opcode_table[0x06] = loadReg;
    opcode_table[0x16] = loadReg;
    opcode_table[0x1E] = loadReg;
    opcode_table[0x26] = loadReg;
    opcode_table[0x2E] = loadReg;
    opcode_table[0xC2] = loadReg;
    opcode_table[0xCA] = loadReg;
    opcode_table[0xD2] = loadReg;
    opcode_table[0xDA] = loadReg;
    opcode_table[0xE9] = loadReg;
    opcode_table[0x18] = loadReg;
    opcode_table[0x20] = loadReg;
    opcode_table[0x28] = loadReg;
    opcode_table[0x30] = loadReg;
    opcode_table[0x38] = loadReg;
    opcode_table[0xC7] = loadReg;
    opcode_table[0xCF] = loadReg;
    opcode_table[0xD7] = loadReg;
    opcode_table[0xDF] = loadReg;
    opcode_table[0xE7] = loadReg;
    opcode_table[0xEF] = loadReg;
    opcode_table[0xF7] = loadReg;
    opcode_table[0xFF] = loadReg;
    opcode_table[0xCD] = loadReg;
    opcode_table[0xC4] = loadReg;
    opcode_table[0xCC] = loadReg;
    opcode_table[0xD4] = loadReg;
    opcode_table[0xDC] = loadReg;
    opcode_table[0xC9] = loadReg;
    opcode_table[0xC0] = loadReg;
    opcode_table[0xC8] = loadReg;
    opcode_table[0xD0] = loadReg;
    opcode_table[0xD8] = loadReg;
    opcode_table[0xD9] = loadReg;
}

void handle_opcode(Z80_State *cpu, uint8_t opcode)
{

    if (opcode == 0xCB)
    {
        uint8_t cb_opcode = cpu->mmu->cart_memory[++cpu->PC];
        handler_cb(cpu, cb_opcode);
    }
    else
    {
        if (opcode_table[opcode])
        {
            opcode_table[opcode](cpu, opcode);
        }
        else
        {
            fprintf(stderr, "Unhandled opcode: 0x%02X at PC: 0x%04X\n", opcode, cpu->PC);
            exit(1);
        }
    }
}

void execute_cycle(Z80_State *cpu)
{
    while (!cpu->Halt)
    {
        // Fetch
        uint8_t opcode = cpu->mmu->cart_memory[cpu->PC];
        // inc PC
        cpu->PC++;
        // Handle and execute instr
        handle_opcode(cpu, opcode);
    }
}
