#include <stdint.h>

#define MEMORY_SIZE 65536 // Z80 has a 16-bit address bus, so 65536 addresses (64KB) are available

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

//Struct represents the state of the CPU and Memory Space
typedef struct {
  //Memory Space of Z80
  uint8_t memory[MEMORY_SIZE]; 
  //8-bit registers
  uint8_t A, B, C, D, E, H, L, I, R; 
  union {
    uint8_t F; // Full 8-bit F register
    Flags flags; // Individual flags
  };
  uint8_t interrupt;

  // 16-bit registers
  uint16_t IX, IY, SP, PC; 

} Z80_State;

