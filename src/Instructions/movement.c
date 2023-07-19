#include <movement.h>
#include <stdbool.h>

 void loadImm(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t imm){
    setReg(cpu,imm);
 }
 void loadMem(Z80_State *cpu, void (*setReg)(Z80_State *, uint8_t), uint8_t offset){
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
        loadMem(cpu, setReg, cpu->getHL);

    }

    uint8_t val = getReg(cpu);
    setReg(cpu, val);
 }