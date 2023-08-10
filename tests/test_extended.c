#include <CUnit/CUnit.h>
#include <extended.h>

void test_swap() {
    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 0x53;
    handler_cb(&cpu, 0x37);
    CU_ASSERT_EQUAL(cpu.AF.A, 0x35);
    CU_ASSERT_EQUAL(cpu.AF.flags.Z, 0); 
}

int add_extended_tests(CU_pSuite suite) {
    if (NULL == CU_add_test(suite, "Test Swap", test_swap)) {
        return -1;
    }

    return 0;
}