#include <CUnit/CUnit.h>
#include "../Instructions/arithmetic.h"
#include "cpu.h"

void test_add8() {
     printf("bruh\n");
    Z80_State cpu;
    printf("bruh\n");
    initCPU(&cpu);
    cpu.A = 10;
    printf("Hi\n");
    add8(&cpu, 5);
    CU_ASSERT_EQUAL(cpu.A, 15);
    
}

void add_arithmetic_tests(CU_pSuite suite) {
    
    CU_add_test(suite, "Test Add8", test_add8);
    
}
