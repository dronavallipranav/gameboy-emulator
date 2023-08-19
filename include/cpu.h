#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdbool.h>
#include <mmu.h>

// 64KB of memory in Z80
#define MEMORY_SIZE 65536

typedef struct
{
  uint8_t S : 1;     // Sign
  uint8_t Z : 1;     // Zero
  uint8_t fifth : 1; // unused
  uint8_t H : 1;     // Half Carry
  uint8_t third : 1; // unused
  uint8_t PV : 1;    // Parity/Overflow
  uint8_t N : 1;     // Add/Subtract
  uint8_t C : 1;     // Carry
} Flags;

typedef struct
{
  uint8_t B, C;
} BC_Register;

typedef struct
{
  uint8_t D, E;
} DE_Register;

typedef struct
{
  uint8_t H, L;
} HL_Register;

typedef struct
{
  uint8_t A;
  union
  {
    uint8_t F;
    Flags flags; // Individual flags
  };
} AF_Register;

typedef struct Z80_State
{

  // Create mmu associated with CPU
  MMU *mmu;
  // Memory Space of Z80
  uint8_t memory[MEMORY_SIZE];
  // 8-bit registers
  uint8_t I, R;

  // Unions represent 16-bit registers as well as 8 bit pairs that make them up
  union
  {
    AF_Register AF;
    uint16_t AF_pair;
  };

  uint8_t (*getA)(struct Z80_State *);
  void (*setA)(struct Z80_State *, uint8_t val);
  uint8_t (*getF)(struct Z80_State *);
  void (*setF)(struct Z80_State *, uint8_t val);
  uint16_t (*getAF)(struct Z80_State *);
  void (*setAF)(struct Z80_State *, uint16_t val);

  union
  {
    BC_Register BC;
    uint16_t BC_pair;
  };
  uint8_t (*getB)(struct Z80_State *);
  void (*setB)(struct Z80_State *, uint8_t val);
  uint8_t (*getC)(struct Z80_State *);
  void (*setC)(struct Z80_State *, uint8_t val);
  uint16_t (*getBC)(struct Z80_State *);
  void (*setBC)(struct Z80_State *, uint16_t val);

  union
  {
    DE_Register DE;
    uint16_t DE_pair;
  };
  uint8_t (*getD)(struct Z80_State *);
  void (*setD)(struct Z80_State *, uint8_t val);
  uint8_t (*getE)(struct Z80_State *);
  void (*setE)(struct Z80_State *, uint8_t val);
  uint16_t (*getDE)(struct Z80_State *);
  void (*setDE)(struct Z80_State *, uint16_t val);

  union
  {
    HL_Register HL;
    uint16_t HL_pair;
  };
  uint8_t (*getH)(struct Z80_State *);
  void (*setH)(struct Z80_State *, uint8_t val);
  uint8_t (*getL)(struct Z80_State *);
  void (*setL)(struct Z80_State *, uint8_t val);
  uint16_t (*getHL)(struct Z80_State *);
  void (*setHL)(struct Z80_State *, uint16_t val);

  uint8_t interrupt;

  // 16-bit registers
  uint16_t IX, IY, SP, PC;

  uint16_t (*getIX)(struct Z80_State *);
  void (*setIX)(struct Z80_State *, uint16_t val);
  uint16_t (*getIY)(struct Z80_State *);
  void (*setIY)(struct Z80_State *, uint16_t val);
  uint16_t (*getSP)(struct Z80_State *);
  void (*setSP)(struct Z80_State *, uint16_t val);
  uint16_t (*getPC)(struct Z80_State *);
  void (*setPC)(struct Z80_State *, uint16_t val);

  // TO DO: IMPLEMENT CARTRIDGE MBC MATCHING FOR MMU
  uint16_t MBC;

  bool Halt;
  bool Stop;

  uint8_t DI_status;
  bool interrupt_status;
  bool nextInstr;
  uint8_t EI_status;

} Z80_State;

void initCPU(Z80_State *cpu);

#endif
