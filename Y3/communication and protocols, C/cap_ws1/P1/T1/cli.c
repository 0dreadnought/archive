#include "cli.h"
#include "flash_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_fgets.h"

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

    // Handle the FLASH_WRITE command
    if (strcmp(token, "write") == 0) {
        // Parse the address
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\nFLASH_WRITE requires an address and data\n");
            return;
        }
        uint32_t address = atoi(token);

        // Parse the data, assuming it's enclosed in quotes
        token = strtok(NULL, "\"");
        if (token == NULL) {
            printf("\nInvalid data format for FLASH_WRITE\n");
            return;
        }

        // Execute the write operation
        flash_write_safe(address, (uint8_t *)token, sizeof(token));
    }
    // Handle the FLASH_READ command
    else if (strcmp(token, "read") == 0) {
        // Parse the address
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("\nFLASH_READ requires an address and length\n");
            return;
        }
        uint32_t address = atoi(token);


        size_t length = 256; // length of buffer is hard coded so the whole page is visible
        
        uint8_t buffer[length];
        flash_read_safe(address, buffer, length);

        // Output the read data
        printf("\nData: %s\n", buffer);
    }
    // Handle the FLASH_ERASE command
    else if (strcmp(token, "erase") == 0) {
        // Parse the address
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("FLASH_ERASE requires an address\n");
            return;
        }
        uint32_t address = atoi(token);

        // Execute the erase operation
        flash_erase_safe(address);
    }
    else if (strcmp(token, "flush") == 0) {
        flash_flush();
    }
    // Handle unknown commands
    else {
        printf("\nUnknown command\n");
    }
}
