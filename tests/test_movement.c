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
    printf("%hu\n", cpu.HL_pair);
    printf("%hhu\n", cpu.memory[cpu.HL_pair]);
    printf("%hhu\n", cpu.getC(&cpu));
    CU_ASSERT_EQUAL(cpu.getC(&cpu), 15);

    //Test Load (HL), C (loadIntoMem) (opcode 0x77)
    cpu.setHL(&cpu, 10);
    cpu.memory[cpu.HL_pair] = 5;
    cpu.AF.A = 7;
    CU_ASSERT_EQUAL(cpu.memory[10], 5);
    loadReg(&cpu, 0x77);
    CU_ASSERT_EQUAL(cpu.memory[10], 7);

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
    printf("%hu\n", cpu.HL_pair);
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
    printf("bruh\n");
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
