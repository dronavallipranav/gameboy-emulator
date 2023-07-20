#include <cpu.h>

typedef void (*InstructionFunction)(Z80_State*);


InstructionFunction instructionTable[256];  // adjust size as necessary


bool running = true;
while (running) {
    uint8_t opcode = fetch(cpu);
    InstructionFunction instruction = instructionTable[opcode];
    if (instruction == NULL) {
        printf("Unrecognized opcode: 0x%02x\n", opcode);
        break;
    }
    instruction(cpu);
}