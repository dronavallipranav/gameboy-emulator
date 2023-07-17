#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    uint8_t* rom;
    uint8_t* ram; 
    
    int rom_size;
    int ram_size;
    
    int current_rom_bank;
    int current_ram_bank;
    
    bool ram_enabled;
    
} Cartridge;

void load_cartridge(Cartridge* cartridge, const char* filename);

#endif