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


    
}



int add_loadReg_tests(CU_pSuite suite) {
    if (NULL == CU_add_test(suite, "Test LoadReg", test_loadReg)) {
        return -1;
    }
    
    return 0;
}
