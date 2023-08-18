#include <CUnit/CUnit.h>
#include "../Instructions/arithmetic.h"
#include "cpu.h"

void test_add8()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 10;
    cpu.BC.B = 5;
    add8(&cpu, cpu.BC.B, false);
    CU_ASSERT_EQUAL(cpu.AF.A, 15);
}

void test_add16()
{
    Z80_State cpu;
    initCPU(&cpu);
    cpu.HL_pair = 0x0F00;
    cpu.BC_pair = 0x0100;
    add16(&cpu, cpu.setHL, cpu.getHL, cpu.BC_pair);
    CU_ASSERT_EQUAL(cpu.HL_pair, 0x1000);
    CU_ASSERT_TRUE(cpu.AF.flags.N == 0);
    CU_ASSERT_TRUE(cpu.AF.flags.H == 1);
    CU_ASSERT_TRUE(cpu.AF.flags.C == 0);
}

void test_addImm()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 10;
    uint16_t imm = 15;
    addImm(&cpu, imm);
    CU_ASSERT_EQUAL(cpu.AF.A, 25);
}

void test_sub8()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 10;
    cpu.BC.B = 5;
    sub8(&cpu, cpu.BC.B, false);
    CU_ASSERT_EQUAL(cpu.AF.A, 5);
}

void test_sub16()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 30;
    cpu.BC_pair = 20;
    sub16(&cpu, cpu.BC_pair);
    CU_ASSERT_EQUAL(cpu.AF.A, 10);
}

void test_subImm()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 20;
    uint16_t imm = 15;
    subImm(&cpu, imm);
    CU_ASSERT_EQUAL(cpu.AF.A, 5);
}

void test_and8()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 0xF0;
    cpu.BC.B = 0xFF;
    and8(&cpu, cpu.BC.B);
    CU_ASSERT_EQUAL(cpu.AF.A, 0xF0);
    CU_ASSERT_TRUE(cpu.AF.flags.Z == ((cpu.AF.A == 0) ? 1 : 0));
    CU_ASSERT_TRUE(cpu.AF.flags.N == 0);
    CU_ASSERT_TRUE(cpu.AF.flags.H == 1);
    CU_ASSERT_TRUE(cpu.AF.flags.C == 0);
}

void test_or8()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 0xF0;
    cpu.BC.B = 0x0F;
    or8(&cpu, cpu.BC.B);
    CU_ASSERT_EQUAL(cpu.AF.A, 0xFF);
    CU_ASSERT_EQUAL(cpu.AF.A, 0xFF);
    CU_ASSERT_TRUE(cpu.AF.flags.Z == ((cpu.AF.A == 0) ? 1 : 0));
    CU_ASSERT_TRUE(cpu.AF.flags.N == 0);
    CU_ASSERT_TRUE(cpu.AF.flags.H == 0);
    CU_ASSERT_TRUE(cpu.AF.flags.C == 0);
}

void test_xor8()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 0xAA;
    cpu.BC.B = 0xFF;
    xor8(&cpu, cpu.BC.B);
    CU_ASSERT_EQUAL(cpu.AF.A, 0x55);
}

void test_cp8()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 10;
    cpu.BC.B = 5;
    cp8(&cpu, cpu.BC.B);
    CU_ASSERT_EQUAL(cpu.AF.flags.Z, 0);
}

void test_inc8()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 9;
    inc8(&cpu, cpu.setA, cpu.AF.A);
    CU_ASSERT_EQUAL(cpu.AF.A, 10);
}

void test_dec8()
{

    Z80_State cpu;
    initCPU(&cpu);
    cpu.AF.A = 10;
    dec8(&cpu, cpu.setA, cpu.AF.A);
    CU_ASSERT_EQUAL(cpu.AF.A, 9);
}

void test_inc16()
{
    Z80_State cpu;
    initCPU(&cpu);
    cpu.HL_pair = 0x0F00;
    add16(&cpu, cpu.setHL, cpu.getHL, 1);
    CU_ASSERT_EQUAL(cpu.HL_pair, 0x0F01);
}

void test_dec16()
{
    Z80_State cpu;
    initCPU(&cpu);
    cpu.HL_pair = 0x0F00;
    add16(&cpu, cpu.setHL, cpu.getHL, -1);
    CU_ASSERT_EQUAL(cpu.HL_pair, 0x0EFF);
}

int add_arithmetic_tests(CU_pSuite suite)
{
    if (NULL == CU_add_test(suite, "Test Add8", test_add8))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test Add16", test_add16))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test Add16", test_addImm))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test Add8", test_sub8))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test Add16", test_sub16))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test Add16", test_subImm))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test And8", test_and8))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test or8", test_or8))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test xor8", test_xor8))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test cp8", test_cp8))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test inc8", test_inc8))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test dec8", test_dec8))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test inc16", test_inc16))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test dec16", test_dec16))
    {
        return -1;
    }
    return 0;
}
