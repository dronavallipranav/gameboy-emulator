#include <CUnit/CUnit.h>
#include "../Instructions/arithmetic.h"
#include "cpu.h"

void test_add8() {

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 10;
    cpu.BC.B = 5;
    add8(&cpu, cpu.BC.B);
    CU_ASSERT_EQUAL(cpu.AF.A, 15);
    
}

void test_add16() {
    
    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 10;
    cpu.BC_pair = 20;
    add16(&cpu, cpu.BC_pair);
    CU_ASSERT_EQUAL(cpu.AF.A, 30);
    
}

void test_addImm() {
    
    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 10;
    uint16_t imm = 15;
    addImm(&cpu, imm);
    CU_ASSERT_EQUAL(cpu.AF.A, 25);
    
}

void test_sub8() {

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 10;
    cpu.BC.B = 5;
    sub8(&cpu, cpu.BC.B);
    CU_ASSERT_EQUAL(cpu.AF.A, 5);
    
}

void test_sub16() {
    
    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 30;
    cpu.BC_pair = 20;
    sub16(&cpu, cpu.BC_pair);
    CU_ASSERT_EQUAL(cpu.AF.A, 10);
    
}

void test_subImm() {
    
    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 20;
    uint16_t imm = 15;
    subImm(&cpu, imm);
    CU_ASSERT_EQUAL(cpu.AF.A, 5);
    
}




int add_arithmetic_tests(CU_pSuite suite) {
    if (NULL == CU_add_test(suite, "Test Add8", test_add8)) {
        return -1; 
    }

    if (NULL == CU_add_test(suite, "Test Add16", test_add16)) {
        return -1; 
    }

    return 0;
}
