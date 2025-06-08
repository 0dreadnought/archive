#include "protocol.h"
#include <stdint.h>
#include <string.h>



int main() {
    protocol_init();
    sleep_ms(200);

    while (1) {
        uint8_t command[MAX_PAYLOAD_SIZE];
        uint8_t type;
        int len = protocol_receive(&type, command, sizeof(command));
        command[len] = '\0';
        sleep_ms(200);
        if(strcmp(command, "echo") == 0){
            char response[] = "ECHO";
            protocol_send(STR_TYPE,response, sizeof(response));
            int o = 0;
            while (o == 0) {
                o = check_recieve();
            }
        }
        else if(strcmp(command, "listen") == 0){
            char response[] = "LISTEN";
            protocol_send(STR_TYPE,response, sizeof(response));
            check_send();
        }
        else{
            char response[] = "Unkown command";
            protocol_send(STR_TYPE,response, sizeof(response));
        }
        
    }
        
}
