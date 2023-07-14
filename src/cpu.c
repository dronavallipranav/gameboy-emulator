#include <stdint.h>
#include <stdio.h>


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

//Struct represents the state of the CPU and Memory Space
typedef struct {
  //Memory Space of Z80
  uint8_t *memory; 
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

int main(){

  Z80_State *cpu;
  initCPU(cpu);

}

void initCPU(Z80_State *cpu){
    //dynamically allocate and init memory space to 0
    cpu -> memory = calloc(0,MEMORY_SIZE);
    if(cpu -> memory == NULL){
      printf("unable to allocate space for memory space");
    }
    // Initialize registers to zero
    cpu->A = 0;
    cpu->B = 0;
    cpu->C = 0;
    cpu->D = 0;
    cpu->E = 0;
    cpu->H = 0;
    cpu->L = 0;
    cpu->I = 0;
    cpu->R = 0;
    cpu->F = 0;
    cpu->IX = 0;
    cpu->IY = 0;
    cpu->SP = 0;
    cpu->PC = 0;
    cpu->interrupt = 0;

    // Initialize flags to zero
    cpu->flags.S = 0;
    cpu->flags.Z = 0;
    cpu->flags.fifth = 0;
    cpu->flags.H = 0;
    cpu->flags.third = 0;
    cpu->flags.PV = 0;
    cpu->flags.N = 0;
    cpu->flags.C = 0;

}

