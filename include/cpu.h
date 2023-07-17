#ifndef CPU_H
#define CPU_H
#include<stdint.h>

// Z80 has a 16-bit address bus, so 65536 addresses (64KB) are available
#define MEMORY_SIZE 65536

//Struct represents the flags in the F register
typedef struct {
  uint8_t S : 1; // Sign
  uint8_t Z : 1; // Zero
  uint8_t fifth : 1; //unused
  uint8_t H : 1; // Half Carry
  uint8_t third : 1; //unused
  uint8_t PV : 1; // Parity/Overflow
  uint8_t N : 1; // Add/Subtract
  uint8_t C : 1; // Carry
} Flags;

typedef struct {
  uint8_t B, C; // 8-bit BC registers
} BC_Register;

typedef struct {
  uint8_t D, E; // 8-bit DE registers
} DE_Register;

typedef struct {
  uint8_t H, L; // 8-bit HL registers
} HL_Register;

typedef struct {
  uint8_t A; // 8-bit A register
  union {
    uint8_t F; // Full 8-bit F register
    Flags flags; // Individual flags
  };
} AF_Register;

typedef struct {
  //Memory Space of Z80
  uint8_t memory[MEMORY_SIZE]; 
  //8-bit registers
  uint8_t I, R;
   
  //Unions represent 16-bit registers as well as 8 bit pairs that make them up
  union {
    AF_Register AF; 
    uint16_t AF_pair; 
  };
  union {
    BC_Register BC; 
    uint16_t BC_pair; 
  };
  union {
    DE_Register DE; 
    uint16_t DE_pair; 
  };
  union {
    HL_Register HL; 
    uint16_t HL_pair; 
  };
  uint8_t interrupt;

  // 16-bit registers
  uint16_t IX, IY, SP, PC; 

} Z80_State;


void initCPU(Z80_State *cpu);


#endif