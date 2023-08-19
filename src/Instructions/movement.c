#include "movement.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <mmu.h>

/*
This function loads an immediate 8 bit value into specified register using function pointer
@param: imm - 8-bit immediate value
*/
void loadImm(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t imm)
{
   setReg(cpu, imm);
}
/*
This function loads an immediate 16 bit value into specified register using function pointer
@param: imm - 16-bit immediate value
*/
void loadImm16(Z80_State *cpu, void (*setReg)(Z80_State *, uint16_t), uint16_t imm)
{
   setReg(cpu, imm);
}
/*
This function uses a function pointer to generalize loading a value from memory into a register
@param: offset - 16-bit register offset specified by opcode
*/
void loadFromMem(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint16_t addr)
{
   uint8_t val = read_byte(cpu->mmu, addr);
   setReg(cpu, val);
}
/*
This function uses a function pointer to generalize loading a register into memory
@param: offset - 16-bit register offset specified by opcode
*/
void loadIntoMem(Z80_State *cpu, uint8_t (*getReg)(Z80_State *), uint16_t offset)
{
   write_byte(cpu->mmu, offset, getReg(cpu));
}
/*
This function uses a function pointer to generalize loading a register into memory
@param: offset - 16-bit register offset specified by opcode
*/
void loadIntoMem16(Z80_State *cpu, uint16_t (*getReg)(Z80_State *), uint16_t offset)
{
   uint16_t value = getReg(cpu);

   // Break down the 16-bit value into two 8-bit values
   uint8_t lsb = value & 0xFF;
   uint8_t msb = (value >> 8) & 0xFF;

   write_byte(cpu->mmu, offset, lsb);
   write_byte(cpu->mmu, offset + 1, msb);
}

void jump(Z80_State *cpu, uint16_t addr)
{
   cpu->PC = addr;
}

void RST(Z80_State *cpu, uint16_t imm)
{
   cpu->SP -= 2;
   write_byte(cpu->mmu, cpu->SP, (uint8_t)(cpu->PC & 0x00FF));
   write_byte(cpu->mmu, cpu->SP + 1, (uint8_t)((cpu->PC & 0xFF00) >> 8));

   cpu->PC = 0x0000 + imm;
}

void call(Z80_State *cpu)
{
   uint16_t nn = (uint16_t)read_byte(cpu->mmu, cpu->PC) | ((uint16_t)read_byte(cpu->mmu, cpu->PC + 1) << 8);

   cpu->PC += 2;

   // Push next instr onto stack
   cpu->SP -= 2;
   write_byte(cpu->mmu, cpu->SP, (uint8_t)(cpu->PC & 0x00FF));
   write_byte(cpu->mmu, cpu->SP + 1, (uint8_t)((cpu->PC & 0xFF00) >> 8));

   cpu->PC = nn;
}
/*
This function pops two bytes from stack and jumps to addr
@param: cpu - cpu state
*/
void ret(Z80_State *cpu)
{
   uint16_t ret_address = read_byte(cpu->mmu, cpu->SP) | ((uint16_t)read_byte(cpu->mmu, cpu->SP + 1) << 8);
   cpu->SP += 2;
   cpu->PC = ret_address;
}

/*
This function handles all 8-bit loads assuming we are using a little endian system
@param: opcode - code to specify which load instruction
*/
void loadReg(Z80_State *cpu, uint8_t opcode)
{
   uint16_t addr;
   uint8_t val;
   uint8_t msb;
   uint8_t (*getReg)(Z80_State *);
   uint16_t (*getReg16)(Z80_State *);
   void (*setReg)(Z80_State *, uint8_t);
   void (*setReg16)(Z80_State *, uint16_t);
   bool status = true;

   switch (opcode)
   {
   // 16 bit loads
   case 0x01:
      addr = (read_byte(cpu->mmu, cpu->PC + 1) << 8) | read_byte(cpu->mmu, cpu->PC);
      setReg16 = cpu->setBC;
      loadImm16(cpu, setReg16, addr);
      status = false;
      break;
   case 0x11:
      addr = (read_byte(cpu->mmu, cpu->PC + 1) << 8) | read_byte(cpu->mmu, cpu->PC);
      setReg16 = cpu->setDE;
      loadImm16(cpu, setReg16, addr);
      status = false;
      cpu->PC += 1;
      break;
   case 0x21:
      addr = (read_byte(cpu->mmu, cpu->PC + 1) << 8) | read_byte(cpu->mmu, cpu->PC);
      setReg16 = cpu->setHL;
      loadImm16(cpu, setReg16, addr);
      status = false;
      cpu->PC += 1;
      break;
   case 0x31:
      addr = (read_byte(cpu->mmu, cpu->PC + 1) << 8) | read_byte(cpu->mmu, cpu->PC);
      setReg16 = cpu->setSP;
      loadImm16(cpu, setReg16, addr);
      status = false;
      cpu->PC += 1;
      break;
   case 0xF9:
      getReg16 = cpu->getHL;
      setReg16 = cpu->setSP;
      break;
   case 0xF8:
   {
      uint8_t sp_low = cpu->SP & 0xFF;
      int8_t n = read_byte(cpu->mmu, cpu->PC);
      uint16_t res = cpu->SP + n;
      setReg16 = cpu->setHL;
      loadImm16(cpu, setReg16, res);
      cpu->AF.flags.C = ((uint16_t)sp_low + (uint16_t)n) > 0xFF ? 1 : 0;
      cpu->AF.flags.H = ((sp_low & 0xF) + (n & 0xF)) > 0xF ? 1 : 0;
      cpu->AF.flags.Z = 0;
      cpu->AF.flags.N = 0;
      cpu->PC += 1;
      status = false;
   }
   break;

   case 0x08:
      addr = (read_byte(cpu->mmu, cpu->PC + 1) << 8) | read_byte(cpu->mmu, cpu->PC);
      getReg16 = cpu->getSP;
      loadIntoMem16(cpu, getReg16, addr);
      status = false;
      cpu->PC += 1;
      break;

   // STACK ops
   case 0xF5:
      getReg16 = cpu->getAF;
      cpu->SP -= 2;
      write_byte(cpu->mmu, cpu->SP, (uint8_t)(cpu->AF_pair & 0x00FF));
      write_byte(cpu->mmu, cpu->SP + 1, (uint8_t)((cpu->AF_pair & 0xFF00) >> 8));
      status = false;
      break;
   case 0xC5:
      getReg16 = cpu->getBC;
      cpu->SP -= 2;
      write_byte(cpu->mmu, cpu->SP, (uint8_t)(cpu->AF_pair & 0x00FF));
      write_byte(cpu->mmu, cpu->SP + 1, (uint8_t)((cpu->AF_pair & 0xFF00) >> 8));
      status = false;
      break;
   case 0xD5:
      getReg16 = cpu->getDE;
      cpu->SP -= 2;
      write_byte(cpu->mmu, cpu->SP, (uint8_t)(cpu->AF_pair & 0x00FF));
      write_byte(cpu->mmu, cpu->SP + 1, (uint8_t)((cpu->AF_pair & 0xFF00) >> 8));
      status = false;
      break;
   case 0xE5:
      getReg16 = cpu->getHL;
      cpu->SP -= 2;
      write_byte(cpu->mmu, cpu->SP, (uint8_t)(cpu->AF_pair & 0x00FF));
      write_byte(cpu->mmu, cpu->SP + 1, (uint8_t)((cpu->AF_pair & 0xFF00) >> 8));
      status = false;
      break;
   case 0xF1:
      addr = (read_byte(cpu->mmu, cpu->SP + 1) << 8) | read_byte(cpu->mmu, cpu->SP);
      cpu->SP += 2;
      setReg16 = cpu->setAF;
      loadImm16(cpu, setReg16, addr);
      status = false;
      break;
   case 0xC1:
      addr = (read_byte(cpu->mmu, cpu->SP + 1) << 8) | read_byte(cpu->mmu, cpu->SP);
      cpu->SP += 2;
      setReg16 = cpu->setBC;
      loadImm16(cpu, setReg16, addr);
      status = false;
      break;
   case 0xD1:
      addr = (read_byte(cpu->mmu, cpu->SP + 1) << 8) | read_byte(cpu->mmu, cpu->SP);
      cpu->SP += 2;
      setReg16 = cpu->setDE;
      loadImm16(cpu, setReg16, addr);
      status = false;
      break;
   case 0xE1:
      addr = (read_byte(cpu->mmu, cpu->SP + 1) << 8) | read_byte(cpu->mmu, cpu->SP);
      cpu->SP += 2;
      setReg16 = cpu->setHL;
      loadImm16(cpu, setReg16, addr);
      status = false;
      break;

   case 0x02:
      getReg = cpu->getA;
      loadIntoMem(cpu, getReg, cpu->BC_pair);
      status = false;
      break;
   case 0x12:
      getReg = cpu->getA;
      loadIntoMem(cpu, getReg, cpu->DE_pair);
      status = false;
      break;
   case 0x77:
      getReg = cpu->getA;
      loadIntoMem(cpu, getReg, cpu->HL_pair);
      status = false;
      break;
   case 0xEA:
      getReg = cpu->getA;
      addr = (uint16_t)(read_byte(cpu->mmu, cpu->PC + 1) << 8) | read_byte(cpu->mmu, cpu->PC);
      loadIntoMem(cpu, getReg, addr);
      cpu->PC += 1;
      status = false;
      break;
   case 0x3A:
      setReg = cpu->setA;
      loadFromMem(cpu, setReg, cpu->HL_pair);
      status = false;
      // Assumes a little endian system
      cpu->HL_pair--;
      break;
   case 0x32:
      getReg = cpu->getA;
      loadIntoMem(cpu, getReg, cpu->HL_pair);
      status = false;
      cpu->HL_pair--;
      break;
   case 0x2A:
      setReg = cpu->setA;
      loadFromMem(cpu, setReg, cpu->HL_pair);
      status = false;
      cpu->HL_pair++;
      break;
   case 0x22:
      getReg = cpu->getA;
      loadIntoMem(cpu, getReg, cpu->HL_pair);
      status = false;
      cpu->HL_pair++;
      break;
   case 0xE0:
      getReg = cpu->getA;
      addr = read_byte(cpu->mmu, cpu->PC) + 0xFF00;
      loadIntoMem(cpu, getReg, addr);
      status = false;
      cpu->PC += 1;
      break;
   case 0xF0:
      setReg = cpu->setA;
      addr = read_byte(cpu->mmu, cpu->PC) + 0xFF00;
      loadFromMem(cpu, setReg, addr);
      status = false;
      cpu->PC += 1;
      break;

   case 0x7F:
      getReg = cpu->getA;
      setReg = cpu->setA;
      break;
   case 0x78:
      getReg = cpu->getB;
      setReg = cpu->setA;
      break;
   case 0x79:
      getReg = cpu->getC;
      setReg = cpu->setA;
      break;
   case 0x7A:
      getReg = cpu->getD;
      setReg = cpu->setA;
      break;
   case 0x7B:
      getReg = cpu->getE;
      setReg = cpu->setA;
      break;
   case 0x7C:
      getReg = cpu->getH;
      setReg = cpu->setA;
      break;
   case 0x7D:
      getReg = cpu->getL;
      setReg = cpu->setA;
      break;
   case 0x7E:
      setReg = cpu->setA;
      loadFromMem(cpu, setReg, cpu->HL_pair);
      status = false;
      break;
   case 0x0A:
      setReg = cpu->setA;
      loadFromMem(cpu, setReg, cpu->BC_pair);
      status = false;
      break;
   case 0x1A:
      setReg = cpu->setA;
      loadFromMem(cpu, setReg, cpu->DE_pair);
      status = false;
      break;
   case 0xFA:
      val = (read_byte(cpu->mmu, cpu->PC + 1) << 8) | read_byte(cpu->mmu, cpu->PC);
      setReg = cpu->setA;
      loadImm(cpu, setReg, val);
      cpu->PC += 1;
      status = false;
      break;
   case 0x3E:
      val = cpu->memory[cpu->PC];
      setReg = cpu->setA;
      loadImm(cpu, setReg, val);
      cpu->PC += 1;
      status = false;
      break;
   case 0xF2:
      setReg = cpu->setA;
      val = cpu->memory[cpu->BC.C + 0xFF00];
      loadFromMem(cpu, setReg, val);
      break;
   case 0xE2:
      getReg = cpu->getA;
      addr = cpu->BC.C + 0xFF00;
      loadIntoMem(cpu, getReg, addr);
      break;

   case 0x40:
      setReg = cpu->setB;
      getReg = cpu->getB;
      break;
   case 0x41:
      setReg = cpu->setB;
      getReg = cpu->getC;
      break;
   case 0x42:
      setReg = cpu->setB;
      getReg = cpu->getD;
      break;
   case 0x43:
      setReg = cpu->setB;
      getReg = cpu->getE;
      break;
   case 0x44:
      setReg = cpu->setB;
      getReg = cpu->getH;
      break;
   case 0x45:
      setReg = cpu->setB;
      getReg = cpu->getL;
      break;
   case 0x46:
      setReg = cpu->setB;
      loadFromMem(cpu, setReg, cpu->HL_pair);
      status = false;
      break;
   case 0x47:
      setReg = cpu->setB;
      getReg = cpu->getA;
      break;

   case 0x48:
      setReg = cpu->setC;
      getReg = cpu->getB;
      break;
   case 0x49:
      setReg = cpu->setC;
      getReg = cpu->getC;
      break;
   case 0x4A:
      setReg = cpu->setC;
      getReg = cpu->getD;
      break;
   case 0x4B:
      setReg = cpu->setC;
      getReg = cpu->getE;
      break;
   case 0x4C:
      setReg = cpu->setC;
      getReg = cpu->getH;
      break;
   case 0x4D:
      setReg = cpu->setC;
      getReg = cpu->getL;
      break;
   case 0x4E:
      setReg = cpu->setC;
      loadFromMem(cpu, setReg, cpu->HL_pair);
      status = false;
      break;
   case 0x4F:
      setReg = cpu->setC;
      getReg = cpu->getA;
      break;

   case 0x50:
      setReg = cpu->setD;
      getReg = cpu->getB;
      break;
   case 0x51:
      setReg = cpu->setD;
      getReg = cpu->getC;
      break;
   case 0x52:
      setReg = cpu->setD;
      getReg = cpu->getD;
      break;
   case 0x53:
      setReg = cpu->setD;
      getReg = cpu->getE;
      break;
   case 0x54:
      setReg = cpu->setD;
      getReg = cpu->getH;
      break;
   case 0x55:
      setReg = cpu->setD;
      getReg = cpu->getL;
      break;
   case 0x56:
      setReg = cpu->setD;
      loadFromMem(cpu, setReg, cpu->HL_pair);
      status = false;
      break;
   case 0x57:
      setReg = cpu->setD;
      getReg = cpu->getA;
      break;

   case 0x58:
      setReg = cpu->setE;
      getReg = cpu->getB;
      break;
   case 0x59:
      setReg = cpu->setE;
      getReg = cpu->getC;
      break;
   case 0x5A:
      setReg = cpu->setE;
      getReg = cpu->getD;
      break;
   case 0x5B:
      setReg = cpu->setE;
      getReg = cpu->getE;
      break;
   case 0x5C:
      setReg = cpu->setE;
      getReg = cpu->getH;
      break;
   case 0x5D:
      setReg = cpu->setE;
      getReg = cpu->getL;
      break;
   case 0x5E:
      setReg = cpu->setE;
      loadFromMem(cpu, setReg, cpu->HL_pair);
      status = false;
      break;

   case 0x60:
      setReg = cpu->setH;
      getReg = cpu->getB;
      break;
   case 0x61:
      setReg = cpu->setH;
      getReg = cpu->getC;
      break;
   case 0x62:
      setReg = cpu->setH;
      getReg = cpu->getD;
      break;
   case 0x63:
      setReg = cpu->setH;
      getReg = cpu->getE;
      break;
   case 0x64:
      setReg = cpu->setH;
      getReg = cpu->getH;
      break;
   case 0x65:
      setReg = cpu->setH;
      getReg = cpu->getL;
      break;
   case 0x66:
      setReg = cpu->setH;
      loadFromMem(cpu, setReg, cpu->HL_pair);
      status = false;
      break;
   case 0x67:
      setReg = cpu->setH;
      getReg = cpu->getA;
      break;

   case 0x68:
      setReg = cpu->setL;
      getReg = cpu->getB;
      break;
   case 0x69:
      setReg = cpu->setL;
      getReg = cpu->getC;
      break;
   case 0x6A:
      setReg = cpu->setL;
      getReg = cpu->getD;
      break;
   case 0x6B:
      setReg = cpu->setL;
      getReg = cpu->getE;
      break;
   case 0x6C:
      setReg = cpu->setL;
      getReg = cpu->getH;
      break;
   case 0x6D:
      setReg = cpu->setL;
      getReg = cpu->getL;
      break;
   case 0x6E:
      setReg = cpu->setL;
      loadFromMem(cpu, setReg, cpu->HL_pair);
      status = false;
      break;
   case 0x6F:
      setReg = cpu->setL;
      getReg = cpu->getA;
      break;

   case 0x06:
      loadImm(cpu, cpu->setB, read_byte(cpu->mmu, cpu->PC));
      cpu->PC += 1;
      break;

   case 0x0E:
      loadImm(cpu, cpu->setC, read_byte(cpu->mmu, cpu->PC));
      cpu->PC += 1;
      break;

   case 0x16:
      loadImm(cpu, cpu->setD, read_byte(cpu->mmu, cpu->PC));
      cpu->PC += 1;
      break;

   case 0x1E:
      loadImm(cpu, cpu->setE, read_byte(cpu->mmu, cpu->PC));
      cpu->PC += 1;
      break;

   case 0x26:
      loadImm(cpu, cpu->setH, read_byte(cpu->mmu, cpu->PC));
      cpu->PC += 1;
      break;

   case 0x2E:
      loadImm(cpu, cpu->setL, read_byte(cpu->mmu, cpu->PC));
      cpu->PC += 1;
      break;

   // JP
   case 0xC3:
      val = read_byte(cpu->mmu, cpu->PC);
      msb = read_byte(cpu->mmu, cpu->PC + 1);
      jump(cpu, (msb << 8) | val);
      status = false;
      break;

   case 0xC2:
      status = false;
      if (cpu->AF.flags.Z == 1)
      {
         cpu->PC += 2;
         break;
      }
      val = read_byte(cpu->mmu, cpu->PC);
      msb = read_byte(cpu->mmu, cpu->PC + 1);
      jump(cpu, (msb << 8) | val);
      break;

   case 0xCA:
      status = false;
      if (cpu->AF.flags.Z == 0)
      {
         cpu->PC += 2;
         break;
      }
      val = read_byte(cpu->mmu, cpu->PC);
      msb = read_byte(cpu->mmu, cpu->PC + 1);
      jump(cpu, (msb << 8) | val);
      break;

   case 0xD2:
      status = false;
      if (cpu->AF.flags.C == 1)
      {
         cpu->PC += 2;
         break;
      }
      val = read_byte(cpu->mmu, cpu->PC);
      msb = read_byte(cpu->mmu, cpu->PC + 1);
      jump(cpu, (msb << 8) | val);
      break;

   case 0xDA:
      status = false;
      if (cpu->AF.flags.C == 0)
      {
         cpu->PC += 2;
         break;
      }
      val = read_byte(cpu->mmu, cpu->PC);
      msb = read_byte(cpu->mmu, cpu->PC + 1);
      jump(cpu, (msb << 8) | val);
      break;

   case 0xE9:
      cpu->PC = read_byte(cpu->mmu, cpu->HL_pair);
      break;

   // JR
   case 0x18:
      val = read_byte(cpu->mmu, cpu->PC);
      cpu->PC = cpu->PC + val;
      break;

   case 0x20:
      status = false;
      if (cpu->AF.flags.Z == 1)
      {
         cpu->PC += 1;
         break;
      }
      val = read_byte(cpu->mmu, cpu->PC);
      cpu->PC = cpu->PC + val;
      break;

   case 0x28:
      status = false;
      if (cpu->AF.flags.Z == 0)
      {
         cpu->PC += 1;
         break;
      }
      val = read_byte(cpu->mmu, cpu->PC);
      cpu->PC = cpu->PC + val;
      break;

   case 0x30:
      status = false;
      if (cpu->AF.flags.C == 1)
      {
         cpu->PC += 1;
         break;
      }
      val = read_byte(cpu->mmu, cpu->PC);
      cpu->PC = cpu->PC + val;
      break;

   case 0x38:
      status = false;
      if (cpu->AF.flags.C == 0)
      {
         cpu->PC += 1;
         break;
      }
      val = read_byte(cpu->mmu, cpu->PC);
      cpu->PC = cpu->PC + val;
      break;

      // RST
   case 0xC7:
      RST(cpu, 0x00);
      break;

   case 0xCF:
      RST(cpu, 0x08);
      break;

   case 0xD7:
      RST(cpu, 0x10);
      break;

   case 0xDF:
      RST(cpu, 0x18);
      break;

   case 0xE7:
      RST(cpu, 0x20);
      break;

   case 0xEF:
      RST(cpu, 0x28);
      break;

   case 0xF7:
      RST(cpu, 0x30);
      break;

   case 0xFF:
      RST(cpu, 0x38);
      break;

   // CALL
   case 0xCD:
      call(cpu);
      break;

   case 0xC4:
      if (cpu->AF.flags.Z == 1)
      {
         cpu->PC += 2;
         break;
      }
      call(cpu);
      break;

   case 0xCC:
      if (cpu->AF.flags.Z == 0)
      {
         cpu->PC += 2;
         break;
      }
      call(cpu);
      break;

   case 0xD4:
      if (cpu->AF.flags.C == 1)
      {
         cpu->PC += 2;
         break;
      }
      call(cpu);
      break;

   case 0xDC:
      if (cpu->AF.flags.C == 0)
      {
         cpu->PC += 2;
         break;
      }
      call(cpu);
      break;

   // RET
   case 0xC9:
      ret(cpu);
      break;

   case 0xC0:
      if (cpu->AF.flags.Z == 1)
      {
         cpu->PC += 2;
         break;
      }
      ret(cpu);
      break;

   case 0xC8:
      if (cpu->AF.flags.Z == 0)
      {
         cpu->PC += 2;
         break;
      }
      ret(cpu);
      break;

   case 0xD0:
      if (cpu->AF.flags.C == 1)
      {
         cpu->PC += 2;
         break;
      }
      ret(cpu);
      break;

   case 0xD8:
      if (cpu->AF.flags.C == 0)
      {
         cpu->PC += 2;
         break;
      }
      ret(cpu);
      break;

   case 0xD9:
      ret(cpu);
      cpu->interrupt_status = true;
      break;

   default:
      fprintf(stderr, "Unhandled CB opcode: 0x%02X at PC: 0x%04X\n", opcode, cpu->PC);
      exit(1);
   }
   if (status)
   {
      uint8_t val = getReg(cpu);
      setReg(cpu, val);
   }
}