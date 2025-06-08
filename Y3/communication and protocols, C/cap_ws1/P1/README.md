
# TASK 1 

## Implementation:

All functions take page number to calculate offset, and work with selected page only.
As it turns out, pico erases at least a sector at a time, no matter what.
This was a huge problem for me until I discovered it, as I just couldn't understand why data was erased, when I was targeting other pages.
Although, it is just another reason to use memory segments as measure units for offset.

Everything interesting is within flash_write_safe(), so theres no point on focusing on anything else.
I calculate sector number and sector's page number, coppy whole sector and modify part of it that is needed.
Save whole sector. Done.

*SECTOR_SIZE is 4KB so it is 16 * 256 Bytes pages long.

```c

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

```

flash_read_safe(): coppyies selected page's content into buffer

flash_erase_safe(): calls flash_write_safe() with data = " "

new function: flash_flush: erases everything past FLASH_TARGET_OFFSET called in cli with "flush"


## changes in cli:

#### renamed all commands to keep me sane:

- FLASH_WRITE -> write 
- FLASH_READ -> read 
- FLASH_ERASE -> erase

#### len is not required in FLASH_READ:

since read now allways coppying whole page
buffer is hardcoded to be 256 bytes long

#### address input as page number:

Address had to be supplied as an offset in memory.
Now all memory is written and red in page blocks = 256,
so page number now must be passed to functions instead

example:write 1 dummy_data     ||     read 2   ||     flush


# TASK 2 

Memory is still split into pages. Now write and read for data longer than a page is supported.
Simple implementation, overwrites next pages until all data written.

My struct is made of write counter, 2 flags indicating if page is continuing previous page's data and if page's data will be continued in next page.
And data array.

flash_read_safe() returns selected page as struct, while also printing whole data chain.

## Implementation:

#### erase:

overwrites selected page with empty struct

```c
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
```

#### read:

seeks the starts of the data chain until page's is_continuing flag is false. 
Then prints data of every page after it, until page's to_be_continued flasg is false

returns selected page only

```c
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

```


#### write:

if data is bigger than a page's data continer, trims data and data_len, and calls itself with offset +1 and trimmed inputs.
overwrites selected page with data, increments write_counter by 1

```c
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
memcpy(sector,(void*)(XIP_BASE + addres),SECTOR_SIZE);

sector[sector_addres].write_counter += 1;
sector[sector_addres].is_continuing = is_continuing;
sector[sector_addres].to_be_continued = to_be_c;

for(int i = 0;i<=PAGE_DATA_SIZE+1;i++){
    sector[sector_addres].data[i] = i<data_len ? data[i] : '\0'; //modifying selected page with given data
}

```
