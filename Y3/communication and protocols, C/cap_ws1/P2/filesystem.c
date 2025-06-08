#include "filesystem.h"
#include "flash_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static META meta;
static FS_DIR cur_dir;


// utils ->

// vec->

char_vec cv_init(){
    char_vec tmp;
    tmp.data = NULL;
    tmp.size = 0;
    tmp.volume = 0;
    
    return tmp;
}

void cv_free(char_vec* cv){
    for (size_t i = 0; i < cv->size; i++) {
        free(cv->data[i]);
        cv->data[i] = NULL;
    }
    free(cv->data);
    cv->data = NULL;
    cv->size = 0;
    cv->volume = 0;
}

void cv_push_back(char_vec* cv, uint8_t* data){
    // check if volume enough for new record
    // if not, double allocated memmory
    if (cv->size == cv->volume) {
        size_t tmp_vol = cv->volume == 0 ? 1 : cv->volume * 2;
        uint8_t** tmp_data = (uint8_t**)realloc(cv->data, tmp_vol * sizeof(uint8_t*));
        cv->data = tmp_data;
        cv->volume = tmp_vol;
    }
    size_t len = strlen(data);
    cv->data[cv->size] = (char*)malloc((len + 1) * sizeof(char));

    strcpy(cv->data[cv->size], data);
    
    cv->size++;
}

uint8_t* cv_pop_back(char_vec* cv){
    if(cv->size){
        cv->size--;
        return  cv->data[cv->size];
    }
    return NULL;
}

// <-vec

void print_path(){
    // lookup parent directory until hit root
    // then output
    FileEntry entry = cur_dir.entry;
    char_vec path;
    path = cv_init();
    do{
        cv_push_back(&path,entry.filename);
        get_Entry(entry.parent_dir, &entry);
    }while(entry.parent_dir!=1);

    if(cur_dir.entry.sector_num != meta.root_dir){
        cv_push_back(&path,entry.filename);
    }

    printf("\n[@");
    for(int i = path.size-1; i>=0;i--){
        printf("/%s",path.data[i]);
    }
    printf("]: ");

    cv_free(&path);
}


char_vec parse_path(const char* path){
    // breaks up path into names by '/'
    // root directory can be called by placing '~' at the begining or leaving first name empty
    // ~/name == /name == root/name

    // returns a vector of names

    char_vec p_path = cv_init();
    uint8_t name[NAME_LEN+1];


    if(DEBUG_F){
        printf("\nparsing: %s\n",path);
    }

    int j=0;
    for(int i=0; i<=strlen(path); i++){
        if(DEBUG_F){
            printf("char:%c\n",path[i]);
        }
        if(path[i] == '/' && i == 0){
            cv_push_back(&p_path, "~");
            if(DEBUG_F){
                printf("word%i: '~'\n",p_path.size);
            }            
        }
        else if(path[i] == '/' || (path[i] == '\0'&& j>0)){

            if(!j>0){}

            else{
                name[j] = '\0';
            cv_push_back(&p_path,name);
            if(DEBUG_F){
                printf("word%i: %s\n",p_path.size,name);
            }
            
            j=0;
            }
        }
        else if(path[i]==' '){

        }
        else{
            name[j++] = path[i];
        }
    }
    if(DEBUG_F){
        for(int i =0;i<p_path.size;i++){
            printf("done: %s\n",p_path.data[i]);
        }
    }
    return  p_path;
}


int get_sector_num_from_cvp(const char_vec* cv, uint32_t* buffer){
    // check if irectory with given name exists in the current directory
    // if so move to next directory and name
    // returns sector number from given path if exists  

    FS_DIR target_dir = cur_dir;

    if(DEBUG_F){
        printf("Searching for sector with path: %s\n",*cv->data);
    }

    for(int i = 0; i<cv->size; i++){

        if(!strcmp(cv->data[i], "..")){
            if(DEBUG_F){
                printf("Step back\n");
            }
            read_whole_sector(target_dir.entry.parent_dir,&target_dir);
        }
        else if(!strcmp(cv->data[i], "~")  && i == 0){
            if(DEBUG_F){
                printf("Start from root\n");
            }
            read_whole_sector(meta.root_dir,&target_dir);
        }
        else{
            bool noDir = 1;
            uint8_t name[NAME_LEN+1];
            if(DEBUG_F){
                printf("\nSearching for %s within : %s",cv->data[i],target_dir.entry.filename);
            }
            for(int j=0; j<target_dir.dircount; j++){

                get_FileName(target_dir.dirs[j],name);
                if(DEBUG_F){
                    printf("%s? %i/%i ",name,1+j,target_dir.dircount);
                }

                if(!strcmp(cv->data[i],name)){
                    if(DEBUG_F){
                        printf(" yes\n");
                    }
                    noDir = 0;
                    read_whole_sector(target_dir.dirs[j],&target_dir);
                    break;
                }
                else if(DEBUG_F){
                    printf(" no\n");
                }
            }
            if(noDir){
                printf("\nNo such directory\n");
                return 0;
            }
        }

    }
    if(DEBUG_F){
        printf("Targer sector num: %i\n",target_dir.entry.sector_num);
    }
    *buffer = target_dir.entry.sector_num;

    //  cv_free(cv);

    return 1;
}

int get_sector_num_from_path(const char* path, uint32_t* buffer){
    // nothing of interest

    char_vec cv = parse_path(path);
    
    if(!get_sector_num_from_cvp(&cv, buffer)){
        return 0;
    }

    return 1;
}

int fs_mk_r_dir(){
    // nothing of interest
    // only used in format()

    FileEntry rEntr;
    strncpy(rEntr.filename, "root", NAME_LEN);
    rEntr.filename[NAME_LEN] = '\0';
    rEntr.is_dir = 1;
    rEntr.sector_num = meta.root_dir;
    rEntr.parent_dir = meta.root_dir; 
    rEntr.size = 0;

    FS_DIR rDir;
    rDir.entry = rEntr;
    rDir.filecount = 0;
    rDir.dircount = 0;

    flash_write_safe(meta.root_dir*FLASH_SECTOR_SIZE,(uint8_t*)&rDir,FLASH_SECTOR_SIZE);

    return 1;
}

int get_Entry(uint32_t sector, FileEntry* buffer){
    // coppy file entry of a given sector into buffer
    if(DEBUG_F){
        printf("Reading entry %i\n",sector);
    }
    flash_read_safe(sector*FLASH_SECTOR_SIZE,(uint8_t*)buffer,sizeof(FileEntry));
    return 1;
}

int get_FileName(uint32_t sector, uint8_t* buffer){
    // same, but name only
    if(DEBUG_F){
        printf("Reading name %i\n",sector);
    }
    flash_read_safe(sector*FLASH_SECTOR_SIZE,buffer,sizeof(char[NAME_LEN+1]));
    return 1;
}

uint32_t get_free_sector(){
    // find first free sector in FAT
    // returns free sector if any, else -1

    if(DEBUG_F){
        printf("Looking for free sector\n");
    }
    for(int i=0; i<MAX_SECTORS; i++){
        if(meta.fat[i] == -1){
            if(DEBUG_F){
                printf("Found: %i\n", i);
            }
            return i;
        }
    }
    if(DEBUG_F){
        printf("No free sector found\n");
    }
    return -1;
}

void get_meta(){
    // reads meta into var
    // only used in init
    flash_read_safe(0,(uint8_t*)&meta,sizeof(META));
}

void save_meta(){
    // saves meta into flash
    flash_write_safe(0,(uint8_t*)&meta,sizeof(META));
}

int read_whole_sector(uint32_t sector, uint8_t* buffer){
    // coppyies whole sector into buffer
    if(DEBUG_F){
        printf("Reading sector %i\n",sector);
    }
    flash_read_safe(sector*FLASH_SECTOR_SIZE, buffer, FLASH_SECTOR_SIZE);
    return 1;
}


int rm_dir_safe(uint32_t sector){
    // clears flash, updates parent dir and FAT

    if(sector==0 || sector == meta.root_dir){
        return 0;
    }
    
    FS_DIR target_dir;

    if(DEBUG_F){
        printf("Removing sector %i\n", sector);
    }

    if(!read_whole_sector(sector,(uint8_t*)&target_dir)){
        return 0;
    }

    if(target_dir.dircount || target_dir.filecount){
        printf("Directory is not empty\n");
        return 0;
    }
    else{
        flash_erase_safe(target_dir.entry.sector_num*FLASH_SECTOR_SIZE);
        
        if(DEBUG_F){
            printf("Sector cleared\n");
        }
        meta.fat[target_dir.entry.sector_num] = -1;
        meta.fat_used_count--;

        if(DEBUG_F){
            printf("Updating parent dir\n");
        }

        if(!read_whole_sector(target_dir.entry.parent_dir,(uint8_t*)&target_dir)){
            return 0;
        }
        //target dir = parent of deleted dir from now on
        for(int i = 0;i<target_dir.dircount;i++){
            if(target_dir.dirs[i]==sector){
                target_dir.dircount--;
                target_dir.dirs[i] = target_dir.dirs[target_dir.dircount];
                target_dir.dirs[target_dir.dircount] = NULL;                
                break;
            }
        }

        flash_write_safe(target_dir.entry.sector_num*FLASH_SECTOR_SIZE,(uint8_t*)&target_dir, FLASH_SECTOR_SIZE);

        read_whole_sector(cur_dir.entry.sector_num, (uint8_t*)&cur_dir);
        if(DEBUG_F){
            printf("Done\n");
        }
        
        save_meta();
        if(DEBUG_F){
            printf("FAT updated\n");
        }
        printf("Done\n");
        return 1;
    }
    
}

int rm_file_safe(uint32_t sector){
    // same, but also clears all other sectors used by the file

    FileEntry entry;

    if(DEBUG_F){
        printf("Removing sector %i\n", sector);
    }

    if(!get_Entry(sector,&entry)){
        return 0;
    }
    uint32_t cur_sector = entry.sector_num;


    for(int i = 0; i<opended_files_count; i++){
        if(opended_files[i]->entry.sector_num == entry.sector_num){
            printf("File is in use\n");
        return 0;
        }
    }
    
    uint32_t next_sector;
    
    flash_erase_safe(cur_sector*FLASH_SECTOR_SIZE);
    next_sector = meta.fat[cur_sector];
    meta.fat[cur_sector] = -1;
    cur_sector = next_sector;
    meta.fat_used_count--;


    while(cur_sector != -2){
        flash_erase_safe(cur_sector*FLASH_SECTOR_SIZE);
        next_sector = meta.fat[cur_sector];
        meta.fat[cur_sector] = -1;
        cur_sector = next_sector;
        meta.fat_used_count--;
    };
    save_meta();

    FS_DIR parent;
    if(!read_whole_sector(entry.parent_dir, (uint8_t*)&parent)){
        return 0;
    }

    for(int i = 0;i<parent.filecount;i++){
        if(parent.files[i] == entry.sector_num){               
            parent.filecount--;
            parent.files[i] = parent.files[parent.filecount];
            parent.files[parent.filecount] = NULL;
            break;
        }
    }

    flash_write_safe(parent.entry.sector_num*FLASH_SECTOR_SIZE,(uint8_t*)&parent, FLASH_SECTOR_SIZE);
    read_whole_sector(cur_dir.entry.sector_num, (uint8_t*)&cur_dir);

    if(DEBUG_F){
        printf("Done\n");
    }

    return 1;
}


int parse_mode(FS_FILE* file,const char* mode){
    // nothing of interest

    if(DEBUG_F){
        printf("Comparing %s\n",mode);
    }
    if(DEBUG_F){
        printf("r?\n");
    }

    if(strcmp(mode,"r") == 0){
        if(DEBUG_F){
            printf("yes\n");
        }
        file->is_read_only = 1;
        return 1;
    }
    if(DEBUG_F){
        printf("w?\n");
    }
    if(strcmp(mode,"w") == 0){
        if(DEBUG_F){
            printf("yes\n");
        }
        file->is_read_only = 0;
        return 1;
    }
    if(DEBUG_F){
        printf("Isn't a valid mode\n");
    }

    return 0;
}

uint32_t create_file(uint32_t parent_sector,const uint8_t* name){
    // creates empty file with given name

    uint32_t sector;
    sector = get_free_sector();
    if(sector == -1){
        return sector;
    }

    FileEntry entry;
    FS_DIR parent;
    if(!read_whole_sector(parent_sector, (uint8_t*)&parent)){
        return -1;
    }

    if(parent.filecount >= FILES_PER_DIR){
        return -1;
    }

    strncpy(entry.filename, name, NAME_LEN);
    entry.filename[NAME_LEN] = '\0';

    entry.is_dir = 0;
    entry.sector_num = sector;
    entry.parent_dir = parent_sector;
    entry.size = 0;
    
    FS_FILE_WRITE file;
    memset(&file, 0, sizeof(FS_FILE_WRITE));
    file.entry = entry;

    flash_write_safe(sector*FLASH_SECTOR_SIZE, (uint8_t*)&file, FLASH_SECTOR_SIZE);

    if(DEBUG_F){
        printf("Entry written: S%i FS%i SN%i PD%i\n",entry.size,file.entry.size,entry.sector_num,entry.parent_dir);
    }


    parent.files[parent.filecount] = sector;
    parent.filecount++;
    flash_write_safe(parent.entry.sector_num * FLASH_SECTOR_SIZE, (uint8_t*)&parent, FLASH_SECTOR_SIZE);

    if(DEBUG_F){
        printf("Parent updated\n");
        
    }  

    meta.fat[sector] = -2;
    meta.fat_used_count++;
    save_meta();
    read_whole_sector(cur_dir.entry.sector_num, (uint8_t*)&cur_dir);

    
    if(DEBUG_F){
        printf("FAT updated\n");
    }

    return sector;
}

int check_if_file(const char* path,uint32_t* buffer){
    // check if provided path ends up on file
    // if so sets buffer to sector num of the file
    
    char_vec tmp = parse_path(path);
    uint8_t* name;
    name = cv_pop_back(&tmp);
    FS_DIR parent_dir;
    
    if(DEBUG_F){
        printf("Check if: %s is file\n",name);
    }
    if(tmp.size ==0){
        parent_dir = cur_dir;
    }
    else{

        uint32_t parent_sector;

        if(!get_sector_num_from_cvp(&tmp,&parent_sector)){
            return 0;
        }

        if(!read_whole_sector(parent_sector, (uint8_t*)&parent_dir)){
            return 0;
        }
    }

    uint8_t name_buf[NAME_LEN+1];
    for(int i = 0; i<parent_dir.filecount;i++){
        get_FileName(parent_dir.files[i],name_buf);
        if(!strcmp(name_buf,name)){
            if(DEBUG_F){
                printf("is file\n");
            }
            *buffer = parent_dir.files[i];
            return 1;
        }
    }
    
    return 0;

}

int rm_file_from_dir(uint32_t target_sector,FS_DIR* dir){
    // updates dir
    if(DEBUG_F){
        printf("rm file %i from dir %s\n",target_sector, dir->entry.filename);
    }

    for(int i = 0; i < dir->filecount; i++){
        if(dir->files[i] == target_sector){
            dir->filecount--;
            dir->files[i] = dir->files[dir->filecount];
            dir->files[dir->filecount] = NULL;
            flash_write_safe(dir->entry.sector_num * FLASH_SECTOR_SIZE, (uint8_t*)dir, FLASH_SECTOR_SIZE);
            return 1;
        }
    }
    return 0;
}
int rm_dir_from_dir(uint32_t target_sector,FS_DIR* dir){
    // updates dir
    if(DEBUG_F){
        printf("rm dir %i from dir %s\n",target_sector, dir->entry.filename);
    }
    
    for(int i = 0; i < dir->dircount; i++){
        if(dir->dirs[i] == target_sector){
            dir->dircount--;
            dir->dirs[i] = dir->dirs[dir->dircount];
            dir->dirs[dir->dircount] = NULL;
            flash_write_safe(dir->entry.sector_num * FLASH_SECTOR_SIZE, (uint8_t*)dir, FLASH_SECTOR_SIZE);
            return 1;
        }
    }
    return 0;
}
    

int add_file_to_dir(uint32_t target_sector,FS_DIR* dir){
    // updates dir

    if(DEBUG_F){
        printf("add file %i to dir %s\n",target_sector, dir->entry.filename);
    }

    if(dir->filecount >= FILES_PER_DIR){
        return 0;
    }
    uint8_t name[NAME_LEN+1];
    if(!get_FileName(target_sector, name)){
        return 0;
    }
    if(file_name_in_use(dir, name)){
        return 0;
    }
    dir->files[dir->filecount] = target_sector;
    dir->filecount++;
    flash_write_safe(dir->entry.sector_num * FLASH_SECTOR_SIZE, (uint8_t*)dir, FLASH_SECTOR_SIZE);
    return 1;
}
int add_dir_to_dir(uint32_t target_sector,FS_DIR* dir){
    // updates dir

    if(DEBUG_F){
        printf("add dir %i to dir %s\n",target_sector, dir->entry.filename);
    }

    if(dir->dircount >= FILES_PER_DIR){
        return 0;
    }
    uint8_t name[NAME_LEN+1];
    if(!get_FileName(target_sector, name)){
        return 0;
    }
    if(dir_name_in_use(dir, name)){
        return 0;
    }
    dir->dirs[dir->dircount] = target_sector;
    dir->dircount++;
    flash_write_safe(dir->entry.sector_num * FLASH_SECTOR_SIZE, (uint8_t*)dir, FLASH_SECTOR_SIZE);
    return 1;
}


int dir_name_in_use(FS_DIR* dir, const char* name){
    // check if Dir with give name is in dir

    if(DEBUG_F){
        printf("Check if dir %s is in %s\n",name, dir->entry.filename);
    }

    uint8_t name_buf[NAME_LEN+1];
    for(int i = 0; i<dir->dircount;i++){
        get_FileName(dir->dirs[i],name_buf);
        if(strcmp(name_buf,name) == 0){
            printf("Name is already in use\n");
            return 1;
        }
    }
    return 0;
}

int file_name_in_use(FS_DIR* dir, const char* name){
    // check if file with give name is in dir

    if(DEBUG_F){
        printf("Check if file %s is in %s\n",name, dir->entry.filename);
    }

    uint8_t name_buf[NAME_LEN+1];
    for(int i = 0; i<dir->filecount;i++){
        get_FileName(dir->files[i],name_buf);
        if(strcmp(name_buf,name) == 0){
            printf("Name is already in use\n");
            return 1;
        }
    }
    return 0;
}

// <- utils

/**
 * Opens a file with the specified path and mode.
 *
 * @param path The path of the file to be opened.
 * @param mode The mode in which the file should be opened.
 * @return A pointer to the opened file, or NULL if an error occurred.
 */
FS_FILE* fs_open(const char* path, const char* mode) {
    
    uint32_t target_sector = 0;
    uint8_t* name;
    char_vec tmp = parse_path(path);
    name = cv_pop_back(&tmp);
    
    FS_DIR parent_dir;
    uint32_t parent_sector;

    if(tmp.size == 0){

        parent_dir = cur_dir;
        parent_sector = cur_dir.entry.sector_num;

    }

    if(!get_sector_num_from_cvp(&tmp,&parent_sector)){
        return 0;
    }    
    cv_free(&tmp);

    if(!read_whole_sector(parent_sector, (uint8_t*)&parent_dir)){
        return 0;
    }
        
    uint8_t name_buf[NAME_LEN+1];
    for(int i = 0; i<parent_dir.filecount;i++){
        get_FileName(parent_dir.files[i],name_buf);
        if(!strcmp(name_buf,name)){
            target_sector = parent_dir.files[i];
            break;
        }
    }

    
    if (!target_sector){
        if(DEBUG_F){
            printf("No file found, create\n");
        }
        target_sector = create_file(parent_sector,name);
        
    }
    
    for(int i = 0; i<opended_files_count; i++){
        if(opended_files[i]->entry.sector_num == target_sector){
            if(DEBUG_F){
                printf("File already opened, no actions taken\n");
            }
            return opended_files[i];
        }
    }

        
    if(target_sector == -1){
        return 0;
    }
    
    FS_FILE* File = (FS_FILE*)malloc(sizeof(FS_FILE));
    
    if(!get_Entry(target_sector, &File->entry)){
        return 0;
    }


    
    if(!parse_mode(File, mode)){
        free(File);
        printf("Unexpected mode\n");
        return 0;
    }
    
    File->cursor = 0;

    
    File->current_page = 0;  

    opended_files[opended_files_count] = File;
    opended_files_count++;

    return File;
}

/**
 * Closes the specified file.
 *
 * @param file A pointer to the file to be closed.
 */
void fs_close(FS_FILE* file) {

    for(int i = 0; i<opended_files_count; i++){
        if(opended_files[i] == file){
            opended_files_count--;
            opended_files[i] = opended_files[opended_files_count];
            opended_files[opended_files_count] = NULL;

            if(DEBUG_F){
                printf("File % closed\n", file->entry.sector_num);
            }
            
            free(file);
        }
    }

    // TODO: Implement fs_close
}

/**
 * Reads data from the specified file into the provided buffer.
 *
 * @param file   A pointer to the file from which to read.
 * @param buffer A pointer to the buffer where the read data will be stored.
 * @param size   The maximum number of bytes to read.
 * @return The number of bytes read, or -1 if an error occurred.
 */
int fs_read(FS_FILE* file, void* buffer, int size) {

    for(int i = 0; i<opended_files_count; i++){
        if(opended_files[i] == file){
            break;
        }
        if(i == opended_files_count-1){
            printf("file is not opened\n");
            return -1;
        }
    }
    

    uint32_t bites_to_read = file->entry.size - file->cursor - file->current_page*DATA_PER_SECTOR;
    uint32_t cur_sector = file->entry.sector_num;

    if(DEBUG_F){
        printf("Bites to read:%i   bufSize:%i\n",bites_to_read, size);
    }

    for(int i = 0; i<file->current_page; i++){
        cur_sector = meta.fat[cur_sector];
    }

    if(bites_to_read <= DATA_PER_SECTOR - file->cursor){
        //check if rest of data is within one sector

        if(size <= bites_to_read){
            //read part if buffer isnt big enough
            flash_read_safe(cur_sector*FLASH_SECTOR_SIZE + file->cursor + sizeof(FileEntry), buffer,size);
            return size;
        }

        //read reast of the data
        flash_read_safe(cur_sector*FLASH_SECTOR_SIZE + file->cursor + sizeof(FileEntry), buffer,bites_to_read);
        return bites_to_read;
    }

    uint32_t sectors_left = file->entry.size/DATA_PER_SECTOR - file->current_page;
    size_t bites_red = 0;

    cur_sector = meta.fat[cur_sector];

    if(size < DATA_PER_SECTOR){
        flash_read_safe(cur_sector*FLASH_SECTOR_SIZE + sizeof(FileEntry) + file->cursor, buffer + bites_red, size - file->cursor);
        bites_red += size;
    }
    else{
        size -= DATA_PER_SECTOR;
        flash_read_safe(cur_sector*FLASH_SECTOR_SIZE + sizeof(FileEntry) + file->cursor, buffer + bites_red, DATA_PER_SECTOR - file->cursor);
        cur_sector = meta.fat[cur_sector];
    }

    
    for(int i = 0; i< sectors_left; i++){
        
        if(size < DATA_PER_SECTOR){
            flash_read_safe(cur_sector*FLASH_SECTOR_SIZE + sizeof(FileEntry), buffer + bites_red, size);
            bites_red += size;
            break;
        }
        size -= DATA_PER_SECTOR;
        flash_read_safe(cur_sector*FLASH_SECTOR_SIZE + sizeof(FileEntry), buffer + bites_red, DATA_PER_SECTOR);
        cur_sector = meta.fat[cur_sector];
    }

    return bites_red;

}

/**
 * Writes data from the provided buffer to the specified file.
 *
 * @param file   A pointer to the file to which to write.
 * @param buffer A pointer to the buffer containing the data to be written.
 * @param size   The number of bytes to write.
 * @return The number of bytes written, or -1 if an error occurred.
 */
int fs_write(FS_FILE* file, const void* buffer, int size) {


    for(int i = 0; i<opended_files_count; i++){
        if(opended_files[i] == file){
            break;
        }
        if(i == opended_files_count-1){
            printf("file is not opened\n");
            return -1;
        }
    }

    if(file->is_read_only){
        printf("No rights\n");
        return -1;
    }


    uint32_t taken_sectors = (file->entry.size / DATA_PER_SECTOR) + 1;
    uint32_t updated_sector_sum = (size / DATA_PER_SECTOR) + 1;
    
    uint32_t last_sector = file->entry.sector_num;
    if(DEBUG_F){
        printf("LS:%i\n",last_sector);
    }

    while(meta.fat[last_sector] != -2){
        last_sector = meta.fat[last_sector];
        if(DEBUG_F){
            printf("Sector chain:%i\n",last_sector);
        }
    }
    if(DEBUG_F){
        printf("TS:%i USS:%i LS:%i S%i FS%i\n",taken_sectors,updated_sector_sum,last_sector, size, file->entry.size);
    }
    
    if(taken_sectors < updated_sector_sum){
        //  Allocaate more sectors
        if(DEBUG_F){
            printf("allocating more space\n");
        }
        if(meta.fat_used_count + updated_sector_sum - taken_sectors > MAX_SECTORS){
            printf("No space\n");
            return -1;
        }
        
        uint32_t new_sec;
        for(int i = updated_sector_sum - taken_sectors; i > 0; i--){
            new_sec = get_free_sector();
            meta.fat[last_sector] = new_sec;
            last_sector = new_sec;
            meta.fat_used_count++;
        }
        meta.fat[last_sector] = -2;
        save_meta();
        read_whole_sector(cur_dir.entry.sector_num, (uint8_t*)&cur_dir);
    }


    else if(taken_sectors > updated_sector_sum){
        //  Deallocate extra sectors
        if(DEBUG_F){
            printf("Deallocating space\n");
        }

        int i = 0;
        last_sector = file->entry.sector_num;
        for(; i < updated_sector_sum; i++){
            last_sector = meta.fat[last_sector];
        }
        uint32_t sector_to_clear = meta.fat[last_sector];
        meta.fat[last_sector] = -2;
        {
        uint32_t tmp = last_sector;

        
        for(; i < taken_sectors; i++){
            tmp = meta.fat[sector_to_clear];
            flash_erase_safe(sector_to_clear*FLASH_SECTOR_SIZE);
            meta.fat[sector_to_clear] = -1;
            sector_to_clear = tmp;
            meta.fat_used_count--;
        }
        save_meta();
        }
    }

    FS_FILE_WRITE tmp;

    if(DEBUG_F){
        printf("Saving data\n");
    }

    last_sector = file->entry.sector_num;

    if(DEBUG_F){
        printf("S%i FS%i\n", size, file->entry.size);
    }

    file->entry.size = size;
    tmp.entry = file->entry;

    if(DEBUG_F){
        printf("S%i FS%i\n", size, file->entry.size);
    }

    for(int i = 0; i < updated_sector_sum; i++){
        if((i+1)*DATA_PER_SECTOR > size){
            memcpy(tmp.data, buffer + i*DATA_PER_SECTOR, size-i*DATA_PER_SECTOR);
        }
        else{
            memcpy(tmp.data, buffer + i*DATA_PER_SECTOR, DATA_PER_SECTOR);
        }
        flash_write_safe(last_sector*FLASH_SECTOR_SIZE, (uint8_t*)&tmp, FLASH_SECTOR_SIZE);
        last_sector = meta.fat[last_sector];
    }
    if(DEBUG_F){
        printf("S%i FS%i\n", size, file->entry.size);
    }
    //read_whole_sector(cur_dir.entry.sector_num, &cur_dir);
    return size;
}

/**
 * Sets the file position indicator for the specified file.
 *
 * @param file   A pointer to the file to seek in.
 * @param offset The number of bytes to offset from the specified position.
 * @param whence The position from which to calculate the offset.
 * @return 0 if successful, or -1 if an error occurred.
 */
int fs_seek(FS_FILE* file, long offset, int whence) {

    switch(whence){

        case SEEK_SET:

            if(offset > file->entry.size || offset<0){
                return -1;
            }
            
            file->cursor = offset;
            file->current_page = 0;
            while(file->cursor >= DATA_PER_SECTOR){
                file->current_page++;
                file->cursor -= DATA_PER_SECTOR;
            }
            return 0;
            break;

        case SEEK_CUR:

            if(offset > file->entry.size - file->cursor - file->current_page * DATA_PER_SECTOR && offset > 0){
                return -1;
            }
            if(-offset > file->cursor + file->current_page * DATA_PER_SECTOR && offset < 0){
                return -1;
            }


            file->cursor += offset;

            while(file->cursor >= DATA_PER_SECTOR){
                file->current_page++;
                file->cursor -= DATA_PER_SECTOR;
            }

            while(file->cursor < 0){
                file->current_page--;
                file->cursor += DATA_PER_SECTOR;
            }

            return 0;
            break;

        case SEEK_END:

            if(offset < 0){
                offset = -offset;
            }
            if(offset > file->entry.size){
                return -1;
            }
        
            file->cursor = file->entry.size - offset;
            file->current_page = 0;

            while(file->cursor >= DATA_PER_SECTOR){
                file->current_page++;
                file->cursor -= DATA_PER_SECTOR;
            }

            return 0;
            break;

        default:

            return -1;
            break;

    }
    
    return 0;
}

int fs_format(){

    meta.root_dir = 1;
    meta.fat_used_count = 2;
    
    for(int i = 0; i<MAX_SECTORS; i++){
        meta.fat[i] = -1;
        flash_erase_safe(i*FLASH_SECTOR_SIZE);
    }
    meta.fat[0] = -2;
    meta.fat[1] = -2;

    fs_mk_r_dir();
    
    save_meta();
    read_whole_sector(meta.root_dir, (uint8_t*)&cur_dir);
    
    return 1;
}

int fs_init(){

    get_meta();
    opended_files_count = 0;
    read_whole_sector(meta.root_dir,(uint8_t*)&cur_dir);


    return 1;
}

int fs_cd(const char* path){

    FS_DIR target_dir;
    uint32_t dir_sector;

    if(!get_sector_num_from_path(path,&dir_sector)){
        return 0;
    }

    if(!read_whole_sector(dir_sector,(uint8_t*)&target_dir)){
        return 0;
    }

    cur_dir = target_dir;

    if(DEBUG_F){
        printf("Done\n");
    }

    return 1;
}

int fs_ls(const char* path, pair_files_dirs* buffer){

    FS_DIR target_dir;
    uint32_t dir_sector;

    if(!get_sector_num_from_path(path,&dir_sector)){
        return 0;
    }

    if(!read_whole_sector(dir_sector,(uint8_t*)&target_dir)){
        return 0;
    }

    uint8_t name[NAME_LEN+1];

    if(DEBUG_F){
        printf("\nPS:%i  SN:%i  FC:%i  DC:%i\n", target_dir.entry.parent_dir, target_dir.entry.sector_num, target_dir.filecount, target_dir.dircount);
    }

    for(int i =0; i<target_dir.dircount; i++){
        get_FileName(target_dir.dirs[i],name);
        cv_push_back(&buffer->dir_names, name);
    }
    for(int i =0; i<target_dir.filecount; i++){
        get_FileName(target_dir.files[i],name);
        cv_push_back(&buffer->file_names, name);
    }
    
    buffer->dircount = target_dir.dircount;
    buffer->filecount = target_dir.filecount;
    
    return 1;
}



int fs_mkdir(const char* path){

    if(meta.fat_used_count==MAX_SECTORS){
        return 0;
    }

    uint32_t target_parent_dir_sector;
    FS_DIR parent_dir;
    char_vec tmp = parse_path(path);

    if(tmp.size==0){
        
        return 0;
    }

    uint8_t* name;
    name = cv_pop_back(&tmp);
    
    if(tmp.size ==0){

        parent_dir = cur_dir;
        target_parent_dir_sector = cur_dir.entry.sector_num;
    }
    else{

        if(!get_sector_num_from_cvp(&tmp, &target_parent_dir_sector)){
            return 0;
        }
                
        if(!read_whole_sector(target_parent_dir_sector, (uint8_t*)&parent_dir)){
            return 0;
        }
    }

    if(DEBUG_F){
        printf("Parent dir: %i\n", target_parent_dir_sector);
    }

    if(parent_dir.dircount >= FILES_PER_DIR){
        printf("Max directory count reached\n");
        return 0;
    }

    if(dir_name_in_use(&parent_dir,name)){
        return 0;
    }

    uint32_t new_dir_sector = get_free_sector();
    if (new_dir_sector == -1){
        return 0;
    }

    parent_dir.dirs[parent_dir.dircount] = new_dir_sector;
    parent_dir.dircount++;

    if(DEBUG_F){
        printf("Parent new dircount:%i",parent_dir.dircount);
    }

    
    FileEntry dir_Entr;
    strncpy(dir_Entr.filename, name, NAME_LEN); 
    dir_Entr.filename[NAME_LEN] = '\0';
    dir_Entr.is_dir = 1;
    dir_Entr.parent_dir = target_parent_dir_sector; 
    dir_Entr.sector_num = new_dir_sector;
    dir_Entr.size = 0;
    
    FS_DIR new_dir;
    new_dir.entry = dir_Entr;
    new_dir.filecount = 0;
    new_dir.dircount = 0;
    
    if(DEBUG_F){
        printf("NEW dir:\nname:%s\nparent:%i sector:%i\n",dir_Entr.filename,dir_Entr.parent_dir,dir_Entr.sector_num);
    }
    
    meta.fat_used_count++;
    meta.fat[new_dir_sector] = -2;
    save_meta();

    flash_write_safe(target_parent_dir_sector*FLASH_SECTOR_SIZE, (uint8_t*)&parent_dir, FLASH_SECTOR_SIZE);
    if(DEBUG_F){
        printf("Parent updated\n");
    }

    flash_write_safe(new_dir_sector*FLASH_SECTOR_SIZE, (uint8_t*)&new_dir, FLASH_SECTOR_SIZE);
    if(DEBUG_F){
        printf("Directory saved\nDone\n");
    }

    free(name);
    read_whole_sector(cur_dir.entry.sector_num, (uint8_t*)&cur_dir);
    return 1;
}

int fs_rm(const char* path){

    uint32_t target_sector;


    if(!get_sector_num_from_path(path,&target_sector)){

        if(!check_if_file(path, &target_sector)){
            return 0;
        }
        if(DEBUG_F){
            printf("isFile\n");
        }

        return rm_file_safe(target_sector);
    }


    if(DEBUG_F){
        printf("isDir\n");
    }


    return rm_dir_safe(target_sector);

}

int fs_rm_violent(const char* path){
    // VIOLENCE BREEDS VIOLENCE
}

int fs_mv(const char* source_path, const char* dest_path){

    uint32_t target_sector;
    uint32_t dest_sector;
    uint8_t is_file = 0;
    
    if(!get_sector_num_from_path(dest_path,&dest_sector)){
        return 0;
    }

    if(!get_sector_num_from_path(source_path,&target_sector)){
        if(!check_if_file(source_path, &target_sector)){
            return 0;
        }
        is_file = 1;
    }

    if(target_sector == dest_sector){
        return 0;
    }

    FS_FILE_WRITE target;

    if(!read_whole_sector(target_sector, (uint8_t*)&target)){
        return 0;
    }
    
    FS_DIR old_parent_dir;
    if(!read_whole_sector(target.entry.parent_dir, (uint8_t*)&old_parent_dir)){
        return 0;
    }
    
    FS_DIR new_parent_dir;
    if(!read_whole_sector(dest_sector, (uint8_t*)&new_parent_dir)){
        return 0;
    }
    
    FileEntry tmp = new_parent_dir.entry;
    while(tmp.parent_dir != meta.root_dir){
        if(tmp.parent_dir == target_sector){
            return 0;
        }
        get_Entry(tmp.parent_dir, &tmp);
    }
    
    target.entry.parent_dir = dest_sector;
    flash_write_safe(target_sector*FLASH_SECTOR_SIZE,(uint8_t*)&target, FLASH_SECTOR_SIZE);
    
    if(is_file){
        if(!rm_file_from_dir(target_sector,&old_parent_dir)){
            return 0;
        }
        if(!add_file_to_dir(target_sector,&new_parent_dir)){
            return 0;
        }
    }
    else{
        if(!rm_dir_from_dir(target_sector,&old_parent_dir)){
            return 0;
        }
        if(!add_dir_to_dir(target_sector,&new_parent_dir)){
            return 0;
        }
    }

    read_whole_sector(cur_dir.entry.sector_num, (uint8_t*)&cur_dir);
    return 1;
}

int fs_rename(const char* path, const char* name){

    uint32_t target_sector;
    uint8_t is_file;

    if(!get_sector_num_from_path(path,&target_sector)){
        if(!check_if_file(path, &target_sector)){
            return 0;
        }
        is_file = 1;
    }

    FS_FILE_WRITE target;

    if(!read_whole_sector(target_sector, (uint8_t*)&target)){
        return 0;
    }

    char_vec tmp;

    tmp = parse_path(name);
    if(tmp.size != 1){
        printf("Invalid name");
        return 0;
    }

    FS_DIR parent_dir;

    if(DEBUG_F){
        printf("parent: %i\n",target.entry.parent_dir);
    }

    if(!read_whole_sector(target.entry.parent_dir, &parent_dir)){
        return 0;
    }

    if(is_file){
        if(file_name_in_use(&parent_dir, name)){
            return 0;
        }
    }
    else{
        if(dir_name_in_use(&parent_dir, name)){
            return 0;
        }
    }

    strncpy(target.entry.filename, name, NAME_LEN);
    target.entry.filename[NAME_LEN] = '\0';

    flash_write_safe(target.entry.sector_num*FLASH_SECTOR_SIZE,(uint8_t*)&target, FLASH_SECTOR_SIZE);
    read_whole_sector(cur_dir.entry.sector_num, (uint8_t*)&cur_dir);
    return 1;
}

uint32_t get_used_sector_count() {
    return meta.fat_used_count;
}

void debug(){
    DEBUG_F = !DEBUG_F;
    printf("Debug: %i\n",DEBUG_F);
}
