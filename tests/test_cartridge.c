#include <CUnit/CUnit.h>
#include <cartridge.h>

void test_load_cartridge()
{
    Cartridge cartridge;
    // Load the Pokemon Red ROM.
    load_cartridge(&cartridge, "/Users/pranavdronavalli/Downloads/pokemon_red.gb");

    // The ROM size for Pokemon Red is 1MB (1024 KB).
    CU_ASSERT_EQUAL(cartridge.rom_size, 1024 * 1024);

    // The RAM size for Pokemon Red is 32KB.
    CU_ASSERT_EQUAL(cartridge.ram_size, 32 * 1024);

    CU_ASSERT_EQUAL(cartridge.ram_enabled, 0);
}

int add_cartridge_tests(CU_pSuite suite)
{
    if (NULL == CU_add_test(suite, "Test Load Cartridge", test_load_cartridge))
    {
        return -1;
    }

    return 0;
}
