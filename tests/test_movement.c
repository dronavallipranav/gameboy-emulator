#include <CUnit/CUnit.h>
#include <cpu.h>
#include <movement.h>
#include <stdint.h>
void test_loadReg()
{
    Z80_State cpu;
    initCPU(&cpu);
    load_memory_tests(cpu.mmu);
    cpu.setA(&cpu, 0xA0);
    cpu.setB(&cpu, 0xB0);
    // Test LOAD A, B (opcode 0x78)
    loadReg(&cpu, 0x78);
    CU_ASSERT_EQUAL(cpu.getA(&cpu), 0xB0);
    CU_ASSERT_EQUAL(cpu.getB(&cpu), 0xB0);
}

void test_loadFromMem()
{
    Z80_State cpu;
    initCPU(&cpu);
    load_memory_tests(cpu.mmu);
    // Test LOAD C, (HL) (loadFromMem) (opcode 0x4E)
    cpu.setHL(&cpu, (uint16_t)20);
    write_byte(cpu.mmu, cpu.HL_pair, (uint8_t)15);
    loadReg(&cpu, 0x4E);
    CU_ASSERT_EQUAL(cpu.getC(&cpu), (uint8_t)15);
}

void test_loadIntoMem()
{
    Z80_State cpu;
    initCPU(&cpu);
    load_memory_tests(cpu.mmu);
    cpu.setHL(&cpu, 10);
    write_byte(cpu.mmu, cpu.HL_pair, 5);
    cpu.AF.A = 7;
    CU_ASSERT_EQUAL(read_byte(cpu.mmu, cpu.HL_pair), 5);
    loadReg(&cpu, 0x77);
    CU_ASSERT_EQUAL(read_byte(cpu.mmu, cpu.HL_pair), 7);
}

void test_push()
{
    Z80_State cpu;
    initCPU(&cpu);
    load_memory_tests(cpu.mmu);
    // Test PUSH (0xF5)
    cpu.SP = 0xFFFE;
    cpu.AF_pair = 0x3210;
    loadReg(&cpu, 0xF5);
    uint16_t value = (read_byte(cpu.mmu, cpu.SP + 1) << 8) | read_byte(cpu.mmu, cpu.SP);
    CU_ASSERT_EQUAL(value, 0x3210);
    CU_ASSERT_EQUAL(cpu.SP, 0xFFFC);
}

void test_pop()
{
    Z80_State cpu;
    initCPU(&cpu);
    load_memory_tests(cpu.mmu);
    // Test POP (0xC1)
    cpu.SP = 0xFFEC;
    write_byte(cpu.mmu, cpu.SP + 1, 0x44);
    write_byte(cpu.mmu, cpu.SP, 0x78);
    cpu.BC_pair = 0x3333;
    loadReg(&cpu, 0xC1);
    CU_ASSERT_EQUAL(cpu.BC_pair, 0x4478);
}

void test_LDHL()
{
    Z80_State cpu;
    initCPU(&cpu);
    load_memory_tests(cpu.mmu);
    cpu.SP = 0xFFFC;
    // Test LDHL SP,n (opcode 0xF8)
    write_byte(cpu.mmu, cpu.PC, 0x2);
    cpu.AF.flags.C = 1;
    cpu.AF.flags.H = 1;
    cpu.AF.flags.Z = 1;
    cpu.AF.flags.N = 1;
    CU_ASSERT_EQUAL(cpu.AF.flags.C, 1);
    CU_ASSERT_EQUAL(cpu.AF.flags.H, 1);
    CU_ASSERT_EQUAL(cpu.AF.flags.Z, 1);
    CU_ASSERT_EQUAL(cpu.AF.flags.N, 1);
    loadReg(&cpu, 0xF8);
    CU_ASSERT_EQUAL(cpu.HL_pair, 0xFFFE);
    CU_ASSERT_EQUAL(cpu.AF.flags.C, 0);
    CU_ASSERT_EQUAL(cpu.AF.flags.H, 0);
    CU_ASSERT_EQUAL(cpu.AF.flags.Z, 0);
    CU_ASSERT_EQUAL(cpu.AF.flags.N, 0);
    CU_ASSERT_EQUAL(cpu.PC, 1);
}

int add_loadReg_tests(CU_pSuite suite)
{
    if (NULL == CU_add_test(suite, "Test LoadReg", test_loadReg))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test load from mem", test_loadFromMem))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test load into mem", test_loadIntoMem))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test push", test_push))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test pop", test_pop))
    {
        return -1;
    }

    if (NULL == CU_add_test(suite, "Test LDHL", test_LDHL))
    {
        return -1;
    }

    return 0;
}
