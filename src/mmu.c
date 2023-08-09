#include "../include/mmu.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_mmu(MMU *mmu) {
    
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

void load_memory(MMU* mmu, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        exit(1);
    }

    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    
    mmu -> cart_memory = (uint8_t*) malloc(file_size);
    if (mmu->cart_memory == NULL) {
        fprintf(stderr, "Failed to allocate memory for ROM\n");
        exit(1);
    }

    fread(mmu->cart_memory, file_size, 1, file);

    fclose(file);
}