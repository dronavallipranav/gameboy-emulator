#include <movement.h>
#include <stdbool.h>
#include <stdio.h>
 /*
 This function loads an immediate 8 bit value into specified register using function pointer
 @param: imm - 8-bit immediate value
 */
 void loadImm(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t imm){
    setReg(cpu,imm);
 }
  /*
 This function uses a function pointer to generalize loading a value from memory into a register
 @param: offset - 16-bit register offset specified by opcode
 */
 void loadFromMem(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint16_t offset){
    uint8_t val = cpu -> memory[offset];
    setReg(cpu, val);
 }
 /*
 This function uses a function pointer to generalize loading a register into memory
 @param: offset - 16-bit register offset specified by opcode
 */
 void loadIntoMem(Z80_State *cpu, uint8_t(*getReg)(Z80_State *), uint16_t offset){
    cpu -> memory[offset] = getReg;
 }
  /*
 This function handles all 8-bit loads assuming we are using a little endian system
 @param: opcode - code to specify which load instruction
 */
 void loadReg(Z80_State *cpu, uint8_t opcode){

    uint8_t (*getReg)(Z80_State *);
    void (*setReg)(Z80_State *, uint8_t);
    bool status = true;

        switch (opcode) {
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
        uint16_t addr = (cpu->memory[cpu->PC+2] << 8) | cpu->memory[cpu->PC+1];
        loadIntoMem(cpu, getReg, cpu->memory[addr]);
        status = false;
        break;
        case 0x32:
        getReg = cpu->getA;
        loadIntoMem(cpu, getReg, cpu->HL_pair);
        //Assumes a little endian system
        cpu->HL_pair--;

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
        uint16_t addr = (cpu->memory[cpu->PC+2] << 8) | cpu->memory[cpu->PC+1];
        setReg = cpu->setA;
        loadImm(cpu, setReg, cpu->memory[addr]);
        cpu -> PC += 2;
        status = false;
        break;
        case 0x3E:
        uint16_t addr = cpu->memory[cpu->PC+1];
        setReg = cpu->setA;
        loadImm(cpu, setReg, cpu->memory[addr]);
        cpu -> PC += 1;
        status = false;
        break;
        case 0xF2:
        setReg = cpu->setA;
        uint8_t val = cpu->memory[cpu->BC.C + 0xFF00];
        loadFromMem(cpu, setReg, val);
        break;
        case 0xE2:
        getReg = cpu->getA;
        uint16_t addr = cpu->BC.C + 0xFF00;
        loadIntoMem(cpu, setReg, addr);
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

    }
    if(status){
    uint8_t val = getReg(cpu);
    setReg(cpu, val);
    }
 }