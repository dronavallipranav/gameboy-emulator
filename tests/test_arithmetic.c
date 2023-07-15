#include <CUnit/CUnit.h>
#include "../Instructions/arithmetic.h"
#include <../cpu.h>

void test_add8() {
    Z80_State cpu;
    init_cpu(&cpu);
    cpu.A = 10;

    add8(&cpu, 5);
    CU_ASSERT_EQUAL(cpu.A, 15);
}

int main() {
    CU_pSuite suite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    suite = CU_add_suite("z80_cpu_test_suite", NULL, NULL);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (NULL == CU_add_test(suite, "test_add8", test_add8)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the automated interface
    CU_automated_run_tests();

    CU_cleanup_registry();

    return CU_get_error();
}
