#ifndef MMU_H
#define MMU_H
#include <stdlib.h>
#include <stdint.h>
#include "../include/cartridge.h"

#define CART_START 0x0000
#define CART_END 0x7FFF

#define VIDEO_RAM_START 0x8000
#define VIDEO_RAM_END 0x9FFF

#define SWITCH_RAM_START 0xA000
#define SWITCH_RAM_END 0xBFFF

#define INTERNAL_RAM_START 0xC000
#define INTERNAL_RAM_END 0xDFFF

#define ECHO_INTERNAL_RAM_START 0xE000
#define ECHO_INTERNAL_RAM_END 0xFDFF

#define OAM_START 0xFE00
#define OAM_END 0xFE9F

#define EMPTY_START 0xFEA0
#define EMPTY_END 0xFEFF

#define IO_PORT_START 0xFF00
#define IO_PORT_END 0xFF4B

#define EMPTY_NO_IO_START 0xFF4C
#define EMPTY_NO_IO_END 0xFF7F

#define INTERNAL_HIGH_RAM_START 0xFF80
#define INTERNAL_HIGH_RAM_END 0xFFFE

#define CART_SIZE 0x8000
#define VIDEO_RAM_SIZE 0x2000
#define SWITCH_RAM_SIZE 0x2000
#define INTERNAL_RAM_SIZE 0x2000
#define ECHO_INTERNAL_RAM_SIZE 0x1E00
#define OAM_SIZE 0xA0
#define EMPTY_SIZE 0x60
#define IO_PORT_SIZE 0x4C
#define EMPTY_NO_IO_SIZE 0x34
#define INTERNAL_HIGH_RAM_SIZE 0x7F

typedef struct MMU {
    Cartridge *cartridge;
    uint8_t *cart_memory;
    uint8_t video_ram[VIDEO_RAM_SIZE];
    uint8_t switch_ram[SWITCH_RAM_SIZE];
    uint8_t internal_ram[INTERNAL_RAM_SIZE];
    uint8_t echo_internal_ram[ECHO_INTERNAL_RAM_SIZE];
    uint8_t oam[OAM_SIZE];
    uint8_t empty[EMPTY_SIZE];
    uint8_t io_ports[IO_PORT_SIZE];
    uint8_t empty_no_io[EMPTY_NO_IO_SIZE];
    uint8_t internal_high_ram[INTERNAL_HIGH_RAM_SIZE];
    uint8_t interrupt_enable;
} MMU;

void init_mmu(MMU *mmu);

void load_memory(MMU* mmu, const char* filename);

uint8_t read_byte(MMU *mmu, uint16_t addr);

#endif 