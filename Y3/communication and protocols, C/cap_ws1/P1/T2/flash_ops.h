#ifndef FLASH_OPS_H
#define FLASH_OPS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


#define FLASH_TARGET_OFFSET (256 * 1024) // Offset where user data starts (256KB into flash)
#define FLASH_SIZE PICO_FLASH_SIZE_BYTES // Total flash size available
#define PAGE_SIZE 256
#define PAGE_DATA_SIZE PAGE_SIZE - 2*sizeof(bool) - sizeof(uint32_t)-2// last byte is reserved for '\0' another one is not used so its %8
#define SECTOR_SIZE 4096
#define PPS SECTOR_SIZE/PAGE_SIZE // Pages per Sector


typedef struct
{
    uint32_t write_counter;
    bool is_continuing;
    bool to_be_continued;
    uint8_t data[PAGE_DATA_SIZE+2];
}page_data;



void flash_write_safe(uint32_t offset, const uint8_t *data, size_t data_len, bool is_continuing);
page_data flash_read_safe(uint32_t offset);
void flash_erase_safe(uint32_t offset);

#endif // FLASH_OPS_H
