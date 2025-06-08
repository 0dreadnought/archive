#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define FLASH_SECTOR_SIZE 4096
#define MAX_SECTORS 448 // PICO_FLASH_SIZE_BYTES - FLASH_TARGET_OFFSET / FLASH_SECTOR_SIZE
#define MAX_FILES 10  // Maximum number of files in the filesystem

#define NAME_LEN 63

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define DATA_PER_SECTOR (uint32_t) (FLASH_SECTOR_SIZE - sizeof(FileEntry))

static bool DEBUG_F = 0;

// Meta data of any file
typedef struct {
    char filename[NAME_LEN + 1];     // name of the file; 256 is toolong
    
    bool is_dir; // Directory is a special case file
    uint32_t sector_num;
    uint32_t parent_dir; 
    uint16_t size;      // Size of the file
    // uint8_t spacebyte;
} FileEntry;

#define FILES_PER_DIR (FLASH_SECTOR_SIZE - sizeof(FileEntry) - 2*sizeof(uint32_t)) / 8
// 8 = int/char * 2 to split space for files and dirs

typedef struct {
    FileEntry entry; 

    bool is_read_only;
    uint32_t cursor;
    uint32_t current_page;
} FS_FILE;

typedef struct{
    FileEntry entry;
    uint8_t data[DATA_PER_SECTOR];
} FS_FILE_WRITE;

typedef struct{
    FileEntry entry;
    uint32_t filecount;
    uint32_t files[FILES_PER_DIR]; 
    uint32_t dircount;
    uint32_t dirs[FILES_PER_DIR];
} FS_DIR;

typedef struct{
    uint32_t root_dir;
    uint32_t fat_used_count;
    uint32_t fat[MAX_SECTORS]; //-1 = free -2 = EOF else next sector num;
} META;

typedef struct{
    
    uint8_t** data;
    size_t size;
    size_t volume;
    
} char_vec;

typedef struct{
    char_vec dir_names;
    char_vec file_names;
    uint32_t filecount;
    uint32_t dircount;
} pair_files_dirs;

static FS_FILE* opended_files[MAX_FILES];
static uint32_t opended_files_count = 0;

void debug();

FS_FILE* fs_open(const char* path, const char* mode);
void fs_close(FS_FILE* file);
int fs_read(FS_FILE* file, void* buffer, int size);
int fs_write(FS_FILE* file, const void* buffer, int size);
int fs_seek(FS_FILE* file, long offset, int whence);

int fs_format(); // why need path for formating ?

int fs_init();

int fs_ls(const char* path, pair_files_dirs* buffer);

int fs_cd(const char* path);
int fs_mkdir(const char* path);
int fs_rm(const char* path);
int fs_rm_violent(const char* path);
int fs_mv(const char* source_path, const char* dest_path);
int fs_rename(const char* path, const char* name);

void print_path();

uint8_t* cv_pop_back(char_vec* cv);
void cv_push_back(char_vec* cv, uint8_t* data);
char_vec cv_init();
void cv_free(char_vec* cv);

uint32_t get_used_sector_count();

//int fs_mv(const char* old_path, const char* new_path);
//int fs_cp(const char* source_path, const char* dest_path);


#endif // FILESYSTEM_H