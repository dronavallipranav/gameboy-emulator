#ifndef PPU_H
#define PPU_H
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t tile;

    // unused only for CGB
    unsigned cgb : 4;

    unsigned pn : 1;
    unsigned x_flip : 1;
    unsigned y_flip : 1;
    unsigned bgp : 1;

} oam;

typedef struct
{
    oam oam_ram[40];
    uint8_t vram[0x2000];
} ppu;

void ppu_write_oam(ppu *ppu, uint16_t addr, uint8_t val);

uint8_t ppu_read_oam(ppu *ppu, uint16_t addr);

void ppu_vram_write(ppu *ppu, u_int16_t addr, uint8_t val);

uint8_t ppu_vram_read(ppu *ppu, uint16_t addr);

#endif