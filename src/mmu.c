#include "../include/mmu.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_mmu(MMU *mmu)
{

    memset(mmu->video_ram, 0, VIDEO_RAM_SIZE);
    memset(mmu->switch_ram, 0, SWITCH_RAM_SIZE);
    memset(mmu->internal_ram, 0, INTERNAL_RAM_SIZE);
    memset(mmu->echo_internal_ram, 0, ECHO_INTERNAL_RAM_SIZE);
    memset(mmu->oam, 0, OAM_SIZE);
    memset(mmu->empty, 0, EMPTY_SIZE);
    memset(mmu->io_ports, 0, IO_PORT_SIZE);
    memset(mmu->empty_no_io, 0, EMPTY_NO_IO_SIZE);
    memset(mmu->internal_high_ram, 0, INTERNAL_HIGH_RAM_SIZE);
    mmu->interrupt_enable = 0;
}

void load_memory(MMU *mmu, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    mmu->cart_memory = (uint8_t *)malloc(file_size);
    if (mmu->cart_memory == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for ROM\n");
        exit(1);
    }

    fread(mmu->cart_memory, file_size, 1, file);

    fclose(file);
}

void load_memory_tests(MMU *mmu)
{
    mmu->cart_memory = (uint8_t *)malloc(CART_SIZE);
    if (mmu->cart_memory == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for ROM\n");
        exit(1);
    }
    memset(mmu->cart_memory, 0, CART_SIZE);
}

uint8_t read_byte(ppu *ppu, MMU *mmu, uint16_t addr)
{
    if (addr == 0xFF44)
    {
        return 0x90;
    }
    if (addr >= CART_START && addr <= CART_END)
    {
        return mmu->cart_memory[addr];
    }
    else if (addr >= VIDEO_RAM_START && addr <= VIDEO_RAM_END)
    {
        return ppu_vram_read(ppu, addr);
    }
    else if (addr >= SWITCH_RAM_START && addr <= SWITCH_RAM_END)
    {
        return mmu->switch_ram[addr - SWITCH_RAM_START];
    }
    else if (addr >= INTERNAL_RAM_START && addr <= INTERNAL_RAM_END)
    {
        return mmu->internal_ram[addr - INTERNAL_RAM_START];
    }
    else if (addr >= ECHO_INTERNAL_RAM_START && addr <= ECHO_INTERNAL_RAM_END)
    {
        return mmu->internal_ram[addr - ECHO_INTERNAL_RAM_START];
    }
    else if (addr >= OAM_START && addr <= OAM_END)
    {
        return ppu_read_oam(ppu, addr);
    }
    else if (addr >= EMPTY_START && addr <= EMPTY_END)
    {
        return 0xFF;
    }
    else if (addr >= IO_PORT_START && addr <= IO_PORT_END)
    {
        return mmu->io_ports[addr - IO_PORT_START];
    }
    else if (addr >= EMPTY_NO_IO_START && addr <= EMPTY_NO_IO_END)
    {
        return 0xFF;
    }
    else if (addr >= INTERNAL_HIGH_RAM_START && addr <= INTERNAL_HIGH_RAM_END)
    {
        return mmu->internal_high_ram[addr - INTERNAL_HIGH_RAM_START];
    }
    else if (addr == 0xFFFF)
    {
        return mmu->interrupt_enable;
    }
    return 0xFF;
}

void write_byte(ppu *ppu, MMU *mmu, uint16_t addr, uint8_t value)
{
    if (addr >= CART_START && addr <= CART_END)
    {
        mmu->cart_memory[addr] = value;
    }
    else if (addr >= VIDEO_RAM_START && addr <= VIDEO_RAM_END)
    {
        ppu_vram_write(ppu, addr, value);
    }
    else if (addr >= SWITCH_RAM_START && addr <= SWITCH_RAM_END)
    {
        mmu->switch_ram[addr - SWITCH_RAM_START] = value;
    }
    else if (addr >= INTERNAL_RAM_START && addr <= INTERNAL_RAM_END)
    {
        mmu->internal_ram[addr - INTERNAL_RAM_START] = value;
    }
    else if (addr >= ECHO_INTERNAL_RAM_START && addr <= ECHO_INTERNAL_RAM_END)
    {
        mmu->internal_ram[addr - ECHO_INTERNAL_RAM_START] = value;
    }
    else if (addr >= OAM_START && addr <= OAM_END)
    {
        ppu_write_oam(ppu, addr, value);
    }
    else if (addr >= IO_PORT_START && addr <= IO_PORT_END)
    {
        mmu->io_ports[addr - IO_PORT_START] = value;
    }
    else if (addr >= INTERNAL_HIGH_RAM_START && addr <= INTERNAL_HIGH_RAM_END)
    {
        mmu->internal_high_ram[addr - INTERNAL_HIGH_RAM_START] = value;
    }
    else if ((addr >= EMPTY_START && addr <= EMPTY_END) ||
             (addr >= EMPTY_NO_IO_START && addr <= EMPTY_NO_IO_END))
    {
    }
    else
    {
        fprintf(stderr, "Unhandled memory write to addr: 0x%04X\n", addr);
        exit(1);
    }
}
