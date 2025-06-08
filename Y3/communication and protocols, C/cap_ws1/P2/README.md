
# FILESYSTEM

Filesystem for Rpi pico's flash memory.

Turned out to be ridiciluosly huge (~1.5k lines), so I will only outline it. Hopefully it is enough.

Unit tests are included in compiled file and can be started by "test" command using cli.
However, they were not made by me, so they are not included in the submission.

Run FORMAT.uf2 first. It will format the memory so the fyle system can functions.

Run NO_FORMAT.uf2 after, to prevent void of saved data.

## Features

- Directories: create, navigate, list, and remove.
- Files: create, write, read, remove.
- Common: rename, move.
- Path navigation supports absolute paths (starting with `/` or `~` for root) and relative paths (`..` for parent directory).
- Sector allocation: simple FAT stored in flash meta-block to manage used/free sectors.
- Dynamic sector allocation: required space for files calculated after each rewrite to allocate more sectors if needed, or free up unused sectors.
- Custom vector: vector to strore file names, dynamicaly allocates memmory, supports char only.

### used structs:

FileEntry: general file data placed in the begining of every sector
```c
typedef struct {
    char filename[NAME_LEN + 1];     // name of the file; 256 is toolong
    
    bool is_dir; // Directory is a special case file
    uint32_t sector_num;
    uint32_t parent_dir; 
    uint16_t size;      // Size of the file
    // uint8_t spacebyte;
} FileEntry;
```

FS_FILE: Used during runtime to track use of a files
```c
typedef struct {
    FileEntry entry; 

    bool is_read_only;
    uint32_t cursor;
    uint32_t current_page;
} FS_FILE;
```

FS_FILE_WRITE: entry + data = sector_size, all sectors that are part of 1 file share entry
```c
typedef struct{
    FileEntry entry;
    uint8_t data[DATA_PER_SECTOR];
} FS_FILE_WRITE;
```


FS_DIR: Takes up whole sector, files and directories are stored as sector numbers
```c
typedef struct{
    FileEntry entry;
    uint32_t filecount;
    uint32_t files[FILES_PER_DIR]; 
    uint32_t dircount;
    uint32_t dirs[FILES_PER_DIR];
} FS_DIR;
```

META: very first sector. Strores FAT and root_dir num
```c
typedef struct{
    uint32_t root_dir;
    uint32_t fat_used_count;
    uint32_t fat[MAX_SECTORS]; //-1 = free -2 = EOF else next sector num;
} META;
```


char_vec: custom vector type, supports: push_back, pop_back. for char only.
```c
typedef struct{
    
    uint8_t** data;
    size_t size;
    size_t volume;
    
} char_vec;
```


pair_files_dirs: used to store output of fs_ls().
```c
typedef struct{
    char_vec dir_names;
    char_vec file_names;
    uint32_t filecount;
    uint32_t dircount;
} pair_files_dirs;
```

### macros:
```c
FLASH_SECTOR_SIZE 4096
MAX_SECTORS 448 // PICO_FLASH_SIZE_BYTES - FLASH_TARGET_OFFSET / FLASH_SECTOR_SIZE
MAX_FILES 10  // Maximum number of files openned at a time

NAME_LEN 63

SEEK_SET 0
SEEK_CUR 1
SEEK_END 2

DATA_PER_SECTOR (uint32_t) (FLASH_SECTOR_SIZE - sizeof(FileEntry)) 
FILES_PER_DIR (FLASH_SECTOR_SIZE - sizeof(FileEntry) - 2*sizeof(uint32_t)) / 8
// 8 = int/char * 2 to split space for files and dirs
```

### global variables:
```c

static META meta;
static FS_DIR cur_dir;
static bool DEBUG_F = 0;
static FS_FILE* opended_files[MAX_FILES];
static uint32_t opended_files_count = 0;

```


## Supported functions:

```c
int fs_init();
int fs_format();

FS_FILE* fs_open(const char* path, const char* mode);
void fs_close(FS_FILE* file);
int fs_read(FS_FILE* file, void* buffer, int size);
int fs_write(FS_FILE* file, const void* buffer, int size);
int fs_seek(FS_FILE* file, long offset, int whence);

int fs_mkdir(const char* path);
int fs_ls(const char* path, pair_files_dirs* buffer);
int fs_cd(const char* path);

int fs_rm(const char* path);
int fs_mv(const char* source_path, const char* dest_path);
int fs_rename(const char* path, const char* name);

```

## CLI commands:

* format
* ls $path
* mkdir $path
* cd $path
* rm $path
* open $path $mode
* read $file $len
* write $file $data
* seek $file $offset $whence
* mv $what $where
* rename $path $new_name
* debug  | turn on/off debug prints
* test  | run tests
* mem   | print number of sectors used

## Utility functions:
```c
char_vec parse_path(const char* path); // returns path breaked into file names
int get_sector_num_from_cvp(const char_vec* cv, uint32_t* buffer); // 1 if success, 0 if not, final sector number goes into buffer
int get_sector_num_from_path(const char* path, uint32_t* buffer); // same but will also parse path
int get_Entry(uint32_t sector, FileEntry* buffer); // copppies entry into buffer
int get_FileName(uint32_t sector, uint8_t* buffer); // same for filename
int read_whole_sector(uint32_t sector, uint8_t* buffer); // same for whole sector
uint32_t get_free_sector(); // returns first free sector in FAT[] or -1 if none
void save_meta(); // save meta to flash
int rm_dir_safe(uint32_t sector); // 1 if success, 0 if not, for directories
int rm_file_safe(uint32_t sector); // same for files
uint32_t create_file(uint32_t parent_sector,const uint8_t* name); // creates emtpy file, returns new file's sector, -1 if not.
int check_if_file(const char* path,uint32_t* buffer); // checks if path ends up on file and puts file's sector into buffer. 1 if succes, 0 if not
int dir_name_in_use(FS_DIR* dir, const char* name); // check if selected directory already has a directory with selected name
int file_name_in_use(FS_DIR* dir, const char* name); // same for files
```