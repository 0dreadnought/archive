#pragma once
#include "flash_ops.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include <math.h>



//T2



void flash_write_safe(uint32_t offset, const uint8_t *data, size_t data_len, bool is_continuing) {

    if((offset+1) * PAGE_SIZE + FLASH_TARGET_OFFSET >= FLASH_SIZE){
        printf("  selected page out of bounds");
        return;
    }

    page_data sector[PPS]; //sector buffer
    uint32_t sector_num = offset/PPS; //number of the selected sector
    uint32_t sector_addres = offset; // page num within the sector
    while(sector_addres>=PPS){sector_addres -= PPS;}
    uint32_t addres = FLASH_TARGET_OFFSET + sector_num*SECTOR_SIZE; // global address
    bool to_be_c = false;
    if(data_len>PAGE_DATA_SIZE){
        to_be_c = true;
        size_t len = -(PAGE_DATA_SIZE - data_len);
        uint8_t new_data[len];
        // for(int i = 0;i<data_len-PAGE_DATA_SIZE;i++){
        //     new_data[i] = data[PAGE_DATA_SIZE+i];
        // }
        data_len = PAGE_DATA_SIZE;
        memcpy(new_data, &data[PAGE_DATA_SIZE], sizeof(new_data));
        flash_write_safe(offset+1, new_data, sizeof(new_data), true);
    }
    else{
        if(is_continuing);
    }
    
    // for(int i = 0; i<PPS;i++){
    //     memcpy(&sector[i],(void*)(XIP_BASE + addres + i * PAGE_SIZE),PAGE_SIZE);
    //     // coppying current state of the sector
    // }

    memcpy(sector,(void*)(XIP_BASE + addres),SECTOR_SIZE);

    sector[sector_addres].write_counter += 1;
    sector[sector_addres].is_continuing = is_continuing;
    sector[sector_addres].to_be_continued = to_be_c;

    for(int i = 0;i<=PAGE_DATA_SIZE+1;i++){
        sector[sector_addres].data[i] = i<data_len ? data[i] : '\0'; //modifying selected page with given data
    }
    

    uint32_t status = save_and_disable_interrupts();
    flash_range_erase(addres, SECTOR_SIZE); 
    flash_range_program(addres, sector, SECTOR_SIZE); // saving the changes
    restore_interrupts(status);
}

page_data flash_read_safe(uint32_t offset) {
    uint32_t addres = offset * PAGE_SIZE + FLASH_TARGET_OFFSET;
    if(offset+PAGE_SIZE>=FLASH_SIZE){
        printf("  selected page out of bounds");
    }
    else{
        page_data page;

        memcpy(&page, (void *)(XIP_BASE + addres), PAGE_SIZE);

        page_data tmp_page = page;
        while(tmp_page.is_continuing){
            addres -= PAGE_SIZE;
            offset -= 1;
            memcpy(&tmp_page,(void *)(XIP_BASE + addres), PAGE_SIZE);
            if(!tmp_page.to_be_continued){
                addres += PAGE_SIZE;
                offset += 1;
                memcpy(&tmp_page,(void *)(XIP_BASE + addres), PAGE_SIZE);
                break;
            }
        }
        bool last_page_flag = true;
        while (tmp_page.to_be_continued){
            printf("\n\nPage:%i EditCount:%i \nData:\n%s\n",offset,tmp_page.write_counter,tmp_page.data);
            addres += PAGE_SIZE;
            offset += 1;
            memcpy(&tmp_page,(void *)(XIP_BASE + addres), PAGE_SIZE);
            if(!tmp_page.is_continuing){
                last_page_flag = false;
                break;
            }

        }
        if(last_page_flag){printf("\n\nPage:%i EditCount:%i \nData:\n%s\n",offset,tmp_page.write_counter,(tmp_page.data+'\0'));}
            
        

        return page;
    }
}

void flash_erase_safe(uint32_t offset) {
    uint32_t addres = offset * PAGE_SIZE + FLASH_TARGET_OFFSET;
    if(addres+PAGE_SIZE>FLASH_SIZE){
        printf("  selected page out of bounds");
    }
    else{
        page_data sector[PPS];
        uint32_t sector_num = floor(offset/PPS);
        uint32_t sector_addres = offset;
        while(sector_addres>=PPS){sector_addres -= PPS;}
        uint32_t addres = FLASH_TARGET_OFFSET + sector_num*SECTOR_SIZE;

        for(int i = 0; i<PPS;i++){
            memcpy(&sector[i],(void*)(XIP_BASE + addres + i * PAGE_SIZE),PAGE_SIZE);
        }

        page_data page;
        page.write_counter = 0;
        page.to_be_continued = false;
        page.is_continuing = false;
        *page.data = NULL;
    
        sector[sector_addres] = page;
        
        uint32_t status = save_and_disable_interrupts();
        flash_range_erase(addres, SECTOR_SIZE);
        flash_range_program(addres, sector, SECTOR_SIZE);
        restore_interrupts(status);
    }
}

void flash_flush(){
    page_data page;
    page.write_counter = 0;
    page.to_be_continued = false;
    page.is_continuing = false;
    *page.data = NULL;
    page_data sector[PPS];
    for (int i=0;i<PPS;i++){
        sector[i] = page;
    }
    uint32_t status = save_and_disable_interrupts();
    for(int i = 0;i<(FLASH_SIZE - FLASH_TARGET_OFFSET)/SECTOR_SIZE;i++){
        flash_range_erase(FLASH_TARGET_OFFSET + i*SECTOR_SIZE, SECTOR_SIZE);
        flash_range_program(FLASH_TARGET_OFFSET + i*SECTOR_SIZE, sector, SECTOR_SIZE);
    }
    restore_interrupts(status);
}

