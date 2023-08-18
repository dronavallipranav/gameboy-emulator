#include <stdint.h>
#include <stdio.h>
#include "../include/cpu.h"
#include <stdlib.h>
#include <string.h>

uint8_t getA(Z80_State *cpu)
{
  return cpu->AF.A;
}

void setA(Z80_State *cpu, uint8_t val)
{
  cpu->AF.A = val;
}

uint8_t getF(Z80_State *cpu)
{
  return cpu->AF.F;
}

void setB(Z80_State *cpu, uint8_t val)
{
  cpu->BC.B = val;
}

uint8_t getB(Z80_State *cpu)
{
  return cpu->BC.B;
}

void setC(Z80_State *cpu, uint8_t val)
{
  cpu->BC.C = val;
}

uint8_t getC(Z80_State *cpu)
{
  return cpu->BC.C;
}

void setD(Z80_State *cpu, uint8_t val)
{
  cpu->DE.D = val;
}

uint8_t getD(Z80_State *cpu)
{
  return cpu->DE.D;
}

uint8_t getE(Z80_State *cpu)
{
  return cpu->DE.E;
}

void setE(Z80_State *cpu, uint8_t val)
{
  cpu->DE.E = val;
}

uint8_t getH(Z80_State *cpu)
{
  return cpu->HL.H;
}

void setH(Z80_State *cpu, uint8_t val)
{
  cpu->HL.H = val;
}

uint8_t getL(Z80_State *cpu)
{
  return cpu->HL.L;
}

void setL(Z80_State *cpu, uint8_t val)
{
  cpu->HL.L = val;
}

uint16_t getAF(Z80_State *cpu)
{
  return cpu->AF_pair;
}

void setAF(Z80_State *cpu, uint16_t val)
{
  cpu->AF_pair = val;
}

uint16_t getBC(Z80_State *cpu)
{
  return cpu->BC_pair;
}

void setBC(Z80_State *cpu, uint16_t val)
{
  cpu->BC_pair = val;
}

uint16_t getDE(Z80_State *cpu)
{
  return cpu->DE_pair;
}

void setDE(Z80_State *cpu, uint16_t val)
{
  cpu->DE_pair = val;
}

uint16_t getHL(Z80_State *cpu)
{
  return cpu->HL_pair;
}

void setHL(Z80_State *cpu, uint16_t val)
{
  cpu->HL_pair = val;
}

uint16_t getSP(Z80_State *cpu)
{
  return cpu->SP;
}

void setSP(Z80_State *cpu, uint16_t val)
{
  cpu->SP = val;
}

void initCPU(Z80_State *cpu)
{

  // init memory space to 0
  memset(cpu->memory, 0, sizeof(cpu->memory));

  // Initialize registers to zero
  cpu->AF.A = 0;
  cpu->AF.F = 0;
  cpu->BC.B = 0;
  cpu->BC.C = 0;
  cpu->DE.D = 0;
  cpu->DE.E = 0;
  cpu->HL.H = 0;
  cpu->HL.L = 0;
  cpu->I = 0;
  cpu->R = 0;
  cpu->IX = 0;
  cpu->IY = 0;
  cpu->SP = 0xFFFE;
  cpu->PC = 0;
  cpu->MBC = 1;
  cpu->interrupt = 0;

  // Initialize flags to zero
  cpu->AF.flags.S = 0;
  cpu->AF.flags.Z = 0;
  cpu->AF.flags.fifth = 0;
  cpu->AF.flags.H = 0;
  cpu->AF.flags.third = 0;
  cpu->AF.flags.PV = 0;
  cpu->AF.flags.N = 0;
  cpu->AF.flags.C = 0;

  cpu->getA = getA;
  cpu->setA = setA;
  cpu->getB = getB;
  cpu->setB = setB;
  cpu->getC = getC;
  cpu->setC = setC;
  cpu->getD = getD;
  cpu->setD = setD;
  cpu->getE = getE;
  cpu->setE = setE;
  cpu->getH = getH;
  cpu->setH = setH;
  cpu->getL = getL;
  cpu->setL = setL;

  cpu->getHL = getHL;
  cpu->setHL = setHL;
  cpu->getAF = getAF;
  cpu->setAF = setAF;
  cpu->getBC = getBC;
  cpu->setBC = setBC;
  cpu->getDE = getDE;
  cpu->setDE = setDE;
  cpu->getSP = getSP;
  cpu->setSP = setSP;

  cpu->Halt = false;
  cpu->Stop = false;

  cpu->DI_status = false;
  cpu->interrupt_status = true;
  cpu->nextInstr = false;
  cpu->EI_status = 0;
  cpu->mmu = malloc(sizeof(MMU));
  if (cpu->mmu == NULL)
  {
    fprintf(stderr, "Failed to allocate memory for MMU\n");
    exit(1);
  }
  init_mmu(cpu->mmu);
  // load_memory(cpu->mmu, "/Users/pranavdronavalli/Downloads/gb-test-roms-master/cpu_instrs/individual/03-op sp,hl.gb");
}
