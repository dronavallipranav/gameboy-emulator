#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include "test_arithmetic.c"

int main() {
    CU_pSuite suite = NULL;
    printf("hello\n");
    
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    //creates tests suite
    suite = CU_add_suite("z80_cpu_test_suite", NULL, NULL);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    //adds to suite
    if (NULL == CU_add_test(suite, "test_add8", test_add8)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

   
    CU_automated_run_tests();

    CU_cleanup_registry();

    return CU_get_error();
}
