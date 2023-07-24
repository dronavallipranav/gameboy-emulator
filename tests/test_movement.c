#include <CUnit/CUnit.h>
#include <cpu.h>
#include <movement.h>

void test_loadReg() {
    Z80_State cpu;
    initCPU(&cpu);

    cpu.setA(&cpu, 0xA0);
    cpu.setB(&cpu, 0xB0);
    
    // Test LOAD A, B (opcode 0x78)
    loadReg(&cpu, 0x78);
    CU_ASSERT_EQUAL(cpu.getA(&cpu), 0xB0);
    CU_ASSERT_EQUAL(cpu.getB(&cpu), 0xB0); 

    
    //Test LOAD C, (HL) (loadFromMem) (opcode 0x4E)
    cpu.setHL(&cpu, 20);
    cpu.memory[cpu.HL_pair] = 15;
    loadReg(&cpu, 0x4E);
    CU_ASSERT_EQUAL(cpu.getC(&cpu), 15);

    //Test Load (HL), C (loadIntoMem) (opcode 0x77)
    cpu.setHL(&cpu, 10);
    cpu.memory[cpu.HL_pair] = 5;
    cpu.AF.A = 7;
    CU_ASSERT_EQUAL(cpu.memory[10], 5);
    loadReg(&cpu, 0x77);
    CU_ASSERT_EQUAL(cpu.memory[10], 7);

    //Test PUSH (0xF5)
    cpu.SP = 0xFF00;
    cpu.AF_pair = 0x3210;
    loadReg(&cpu, 0xF5);
    uint16_t value = (cpu.memory[cpu.SP + 1] << 8) | cpu.memory[cpu.SP];
    CU_ASSERT_EQUAL(value, 0x3210);
    CU_ASSERT_EQUAL(cpu.SP, 0xFEFE);

    //Test POP (0xC1)
    cpu.SP = 0xFEFE;
    cpu.memory[cpu.SP+1] = 0x44;
    cpu.memory[cpu.SP] = 0x78;
    cpu.BC_pair = 0x3333;
    loadReg(&cpu, 0xC1);
    CU_ASSERT_EQUAL(cpu.BC_pair, 0x4478);

    //Test LDHL SP,n (opcode 0xF8)
    cpu.SP = 0xFF00;
    cpu.memory[1] = 0x2;
    cpu.AF.flags.C = 1;
    cpu.AF.flags.H = 1;
    cpu.AF.flags.Z = 1;
    cpu.AF.flags.N = 1;
    CU_ASSERT_EQUAL(cpu.AF.flags.C, 1);
    CU_ASSERT_EQUAL(cpu.AF.flags.H, 1);
    CU_ASSERT_EQUAL(cpu.AF.flags.Z, 1);
    CU_ASSERT_EQUAL(cpu.AF.flags.N, 1);
    loadReg(&cpu, 0xF8);
    CU_ASSERT_EQUAL(cpu.HL_pair, 0xFF02);
    CU_ASSERT_EQUAL(cpu.AF.flags.C, 0);
    CU_ASSERT_EQUAL(cpu.AF.flags.H, 0);
    CU_ASSERT_EQUAL(cpu.AF.flags.Z, 0);
    CU_ASSERT_EQUAL(cpu.AF.flags.N, 0);
    CU_ASSERT_EQUAL(cpu.PC, 1);
    cpu.PC = 0;
    cpu.SP = 0xFFFF;
    cpu.memory[1] = 0xFF;
    loadReg(&cpu, 0xF8);
    CU_ASSERT_EQUAL(cpu.AF.flags.C, 1);
    CU_ASSERT_EQUAL(cpu.AF.flags.H, 1);
    CU_ASSERT_EQUAL(cpu.AF.flags.Z, 0);
    CU_ASSERT_EQUAL(cpu.AF.flags.N, 0); 
    
}



int add_loadReg_tests(CU_pSuite suite) {
    if (NULL == CU_add_test(suite, "Test LoadReg", test_loadReg)) {
        return -1;
    }
    
    return 0;
}
