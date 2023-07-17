#include "../include/cartridge.h"
#include <stdlib.h>
#include <stdio.h>

/*
* Function looks at byte 0x0149 in cartridge header to allocate RAM size in cartridge (if exists)
*/
void allocate_ram(Cartridge* cartridge) {
    switch (cartridge->rom[0x0149]) {
        case 0x00:
            cartridge->ram = NULL;
            cartridge->ram_size = 0;
            break;
        case 0x01:
            cartridge->ram = (uint8_t*) calloc(2 * 1024, sizeof(uint8_t)); 
            cartridge->ram_size = 2 * 1024;
            break;
        case 0x02:
            cartridge->ram = (uint8_t*) calloc(8 * 1024, sizeof(uint8_t)); 
            cartridge->ram_size = 8 * 1024;
            break;
        case 0x03:
            cartridge->ram = (uint8_t*) calloc(32 * 1024, sizeof(uint8_t));
            cartridge->ram_size = 32 * 1024;
            break;
        case 0x04:
            cartridge->ram = (uint8_t*) calloc(128 * 1024, sizeof(uint8_t)); 
            cartridge->ram_size = 128 * 1024;
            break;
        case 0x05:
            cartridge->ram = (uint8_t*) calloc(64 * 1024, sizeof(uint8_t));
            cartridge->ram_size = 64 * 1024;
            break;
        default:
            fprintf(stderr, "Unknown RAM size code %02x\n", cartridge->rom[0x0149]);
            exit(1);
    }
}

void load_cartridge(Cartridge *cartridge, const char* filename) {

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        exit(1);
    }

    // Get the file size.
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the ROM.
    cartridge->rom = (uint8_t*) malloc(file_size);
    if (cartridge->rom == NULL) {
        fprintf(stderr, "Failed to allocate memory for ROM\n");
        exit(1);
    }
    cartridge->rom_size = file_size;

    //Load the ROM data into memory.
    fread(cartridge->rom, file_size, 1, file);

    fclose(file);

    allocate_ram(cartridge);
    //MBC1 specific
    cartridge->current_rom_bank = 1; 
    cartridge->current_ram_bank = 0;
    cartridge->ram_enabled = 0;

}


