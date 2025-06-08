#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "flash_ops.h"
#include "filesystem.h"
#include "cli.h"
#include "custom_fgets.h"
const uint LED_PIN = 25;

int main() {
    stdio_init_all();
    //fs_format();
    fs_init();
    char command[1024];

    // Wait for USB connection
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // Command loop
    while (1) {
        print_path();
        custom_fgets(command, sizeof(command), stdin);
        execute_command(command);
    }

    return 0;
}
