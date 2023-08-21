#include <ppu.h>

void ppu_write_oam(ppu *ppu, uint16_t addr, uint8_t val)
{
    if (addr >= 0xFE00)
    {
        addr -= 0xFE00;
    }

    uint8_t *temp = (uint8_t *)ppu->oam_ram;
    temp[addr] = val;
}

uint8_t ppu_read_oam(ppu *ppu, uint16_t addr)
{
    if (addr >= 0xFE00)
    {
        addr -= 0xFE00;
    }

    uint8_t *temp = (uint8_t *)ppu->oam_ram;
    return temp[addr];
}

void ppu_vram_write(ppu *ppu, u_int16_t addr, uint8_t val)
{
    ppu->vram[addr - 0x8000] = val;
}

uint8_t ppu_vram_read(ppu *ppu, uint16_t addr)
{
    return ppu->vram[addr - 0x8000];
}