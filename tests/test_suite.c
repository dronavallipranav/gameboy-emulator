#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include "test_arithmetic.c"
#include "test_cartridge.c"
#include "test_movement.c"

int main() {
    CU_pSuite suite = NULL;
    
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    suite = CU_add_suite("z80_cpu_test_suite", NULL, NULL);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (add_arithmetic_tests(suite) != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (add_cartridge_tests(suite) != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (add_loadReg_tests(suite) != 0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();

     if (CU_get_number_of_failures() == 0) {
        printf("All tests passed\n");
    }else{
        printf("tests failed");
    }

    CU_cleanup_registry();

    return CU_get_error();
}