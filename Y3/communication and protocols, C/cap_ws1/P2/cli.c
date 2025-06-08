#include "cli.h"
#include "flash_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_fgets.h"
#include "filesystem.h"
#include "tests.h"

// Function: execute_command
// Parses and executes commands related to flash memory operations.
//
// Parameters:
// - command: A string containing the command and its arguments.
//
// The function supports the following commands:
// - FLASH_WRITE: Writes data to flash memory.
// - FLASH_READ: Reads data from flash memory.
// - FLASH_ERASE: Erases a sector of flash memory.
//
// Each command expects specific arguments following the command name.
void execute_command(char *command) {
    // Split the command string into tokens
    char *token = strtok(command, " ");

    // Check for an empty or invalid command
    if (token == NULL) {
        printf("\nInvalid command\n");
        return;
    }

    if (strcmp(token, "format") == 0){
        fs_format();
    }



    else if (strcmp(token, "ls") == 0) {
        
        pair_files_dirs buf;
        buf.dir_names = cv_init();
        buf.file_names = cv_init();

        token = strtok(NULL, " ");
        if (token == NULL) {
            token = "";
        }

        if(!fs_ls(token, &buf)){
            printf("error\n");
            cv_free(&buf.file_names);
            cv_free(&buf.dir_names);
            return;
        }
        printf("\nD: ");
        for(int i =0; i<buf.dircount; i++){
            printf("%s ",buf.dir_names.data[i]);
        }
        printf("\nF: ");
        for(int i =0; i<buf.filecount; i++){
            printf("%s ",buf.file_names.data[i]);
        }
        cv_free(&buf.file_names);
        cv_free(&buf.dir_names);
        printf("\n");
    }




    else if (strcmp(token, "mkdir") == 0) {

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no path provided\n");
            return;
        }
        if(!fs_mkdir(token)){printf("error\n");return;}
    }




    else if (strcmp(token, "cd") == 0) {

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no path provided\n");
            return;
        }

        if(!fs_cd(token)){printf("error\n");return;}
    }



    else if (strcmp(token, "rm") == 0) {

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no path provided\n");
            return;
        }

        if(!fs_rm(token)){printf("error\n");return;}
    }

    else if (strcmp(token, "open") == 0){
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no path provided\n");
            return;
        }
        FS_FILE* file;
        file = fs_open(token, "r");
        if(file){
        fs_close(file);
        }

    }

    else if (strcmp(token, "read") == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no file selected\n");
            return;
        }

        FS_FILE* file;
        file = fs_open(token,"r");
    
        uint32_t len = atoi(strtok(NULL, " "));
        if (len == NULL) {
            printf("\n no size selected\n");
            return;
        }
        uint8_t buffer[len+1];

        if(fs_read(file, buffer, len)!=len){printf("error\n");}
        fs_close(file);
        buffer[len] = '\0';
        printf("\n%s",buffer);

    }


    else if (strcmp(token, "write") == 0) {

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no file selected\n");
            return;
        }
        FS_FILE* file;
        file = fs_open(token,"w");
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no length provided\n");
            return;
        }
        if(fs_write(file, token, strlen(token))!=strlen(token)){printf("error\n");fs_close(file);return;}
        fs_close(file);


    }

    else if (strcmp(token, "seek") == 0) {

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no file selected\n");
            return;
        }
        uint32_t file_num = atoi(token);
        FS_FILE* file;
        file = fs_open(token,"r");

        uint32_t offset = strtok(NULL, " ");
        if (offset == NULL) {
            printf("\n no offset provided\n");
            return;
        }
        uint32_t whence = strtok(NULL, " ");
        if (whence == NULL) {
            printf("\n no whence provided\n");
            return;
        }

        if(fs_seek(file, atoi(offset), atoi(whence))<0){printf("error\n");fs_close(file);return;}
    
    }

    else if (strcmp(token, "mv") == 0){

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no file selected\n");
            return;
        }
        char* dest_path;
        dest_path = strtok(NULL, " ");

        if(!fs_mv(token, dest_path)){
            printf("error\n");
            return;
        }
    }

    else if (strcmp(token, "rename") == 0){

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\n no file selected\n");
            return;
        }
        char* name;
        name = strtok(NULL, " ");

        if(!fs_rename(token, name)){
            printf("error\n");
            return;
        }
    }

    
    else if (strcmp(token, "test") == 0) {
        run_filesystem_tests();    
    }

    else if (strcmp(token, "debug") == 0){
        //printf("%i %i, %i",DATA_PER_SECTOR,FLASH_SECTOR_SIZE, sizeof(FileEntry));
        debug();
    }

    else if (strcmp(token, "mem") == 0){
        printf("\n%i/%i",get_used_sector_count(), MAX_SECTORS);
    }





    // Handle unknown commands
    else {
        printf("\nUnknown command\n");
    }
}
