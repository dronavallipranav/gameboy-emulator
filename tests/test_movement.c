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
    
}

int add_loadReg_tests(CU_pSuite suite) {
    if (NULL == CU_add_test(suite, "Test LoadReg", test_loadReg)) {
        return -1;
    }

    return 0;
}
