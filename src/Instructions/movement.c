#include <movement.h>
#include <stdbool.h>
#include <stdio.h>
 void loadImm(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t imm){
    setReg(cpu,imm);
 }
 void loadMem(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint16_t offset){
    uint8_t val = cpu -> memory[offset];
    setReg(cpu, val);
 }
 void loadReg(Z80_State *cpu, uint8_t opcode){

    uint8_t (*getReg)(Z80_State *);
    void (*setReg)(Z80_State *, uint8_t);
    bool status = true;

        switch (opcode) {
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
        loadMem(cpu, setReg, cpu->HL_pair);
        status = false;
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
        loadMem(cpu, setReg, cpu->HL_pair);
        status = false;
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
        loadMem(cpu, setReg, cpu->HL_pair);
        status = false;
        break;

    }
    if(status){
    uint8_t val = getReg(cpu);
    setReg(cpu, val);
    }
 }