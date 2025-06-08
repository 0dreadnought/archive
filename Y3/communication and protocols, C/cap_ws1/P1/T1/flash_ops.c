#include "flash_ops.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include <math.h>


#define FLASH_TARGET_OFFSET (256 * 1024) // Offset where user data starts (256KB into flash)
#define FLASH_SIZE PICO_FLASH_SIZE_BYTES // Total flash size available
#define PAGE_SIZE 256
#define SECTOR_SIZE 4096

//T1

void flash_write_safe(uint32_t offset, const uint8_t *data, size_t data_len) {

    if((offset+1) * PAGE_SIZE + FLASH_TARGET_OFFSET > FLASH_SIZE){
        printf("  selected page out of bounds");
        return;
    }

    data_len = PAGE_SIZE;
    uint8_t sector[SECTOR_SIZE]; //sector buffer
    uint32_t sector_num = floor(offset/16); //number of the selected sector
    uint32_t sector_addres = (offset%16) * PAGE_SIZE; // address within the sector
    uint32_t addres = FLASH_TARGET_OFFSET + sector_num*SECTOR_SIZE; // global address
    memcpy(sector,(void *)(XIP_BASE + addres), SECTOR_SIZE); // coppying current state of the sector
    for(int i = 0;i<data_len;i++){
        sector[sector_addres+i] = data[i]; //modifying selected page with given data
    }
    uint32_t status = save_and_disable_interrupts();
    flash_range_erase(addres, SECTOR_SIZE); 
    flash_range_program(addres, sector, SECTOR_SIZE); // saving the changes
    restore_interrupts(status);
}

void flash_read_safe(uint32_t offset, uint8_t *buffer, size_t buffer_len) {
    uint32_t addres = offset * PAGE_SIZE + FLASH_TARGET_OFFSET;
    if(offset+PAGE_SIZE>FLASH_SIZE){
        printf("  selected page out of bounds");
    }
    else{
        memcpy(buffer, (void *)(XIP_BASE + addres), buffer_len);
    }
}

void flash_erase_safe(uint32_t offset) {
    uint32_t addres = offset * PAGE_SIZE + FLASH_TARGET_OFFSET;
    if(addres+PAGE_SIZE>FLASH_SIZE){
        printf("  selected page out of bounds");
    }
    else{
        uint8_t data[256] = " ";
        flash_write_safe(offset, data, 256); // only erase selected page
    }
}

void flash_flush(){
    uint32_t status = save_and_disable_interrupts();
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SIZE - FLASH_TARGET_OFFSET);
    restore_interrupts(status);
}

