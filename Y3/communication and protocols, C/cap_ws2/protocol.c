#include "protocol.h"
#include "pico/stdlib.h"
#include <string.h>


#define BAUD_RATE 9600

#define START_BYTE 0xFE
#define END_BYTE 0xFF

void protocol_init(void) {
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
}

int protocol_connect(const char* address, int port) {
    // No need
    return 1;
}

int protocol_send(uint8_t type, const void* data, int dataSize) {
    const uint8_t* bytes = (const uint8_t*)data;
    int checksum = 0;

    putchar_raw(START_BYTE);
    putchar_raw(type);
    putchar_raw(dataSize);

    for (int i = 0; i < dataSize; i++) {
        putchar_raw(bytes[i]);
        checksum = checksum ^ bytes[i];
    }

    putchar_raw(checksum);
    putchar_raw(END_BYTE);

    return dataSize;
}

int protocol_receive(uint8_t* outType, void* buffer, int bufferSize) {
    while((uint8_t)getchar()!=START_BYTE){
    }
        
    int checksum = 0;
    uint8_t* recv_buffer = (uint8_t*) buffer;
    
    *outType = (uint8_t)getchar();
    int size = (uint8_t)getchar();
    
    for (int i = 0; i < bufferSize ; i++) {
        if (i == size){
            break;
        }
        recv_buffer[i] = (uint8_t)getchar();
        checksum = checksum ^ recv_buffer[i];

    }
    if( checksum != (uint8_t)getchar()){
        return 0;
    }
    if( (uint8_t)getchar() != END_BYTE){
        return 0;
    }

    return bufferSize > size ? size : bufferSize;
}

void protocol_disconnect(int handle) {
    // No need
}

void protocol_cleanup(void) {
    // Nothing to cleanup i think?
}

int check_recieve(void){

    uint8_t recv_buf[MAX_PAYLOAD_SIZE];
    uint8_t type;
    sleep_ms(200);
    uint8_t len = protocol_receive(&type, recv_buf, sizeof(recv_buf));
    sleep_ms(200);
    if (type == STR_TYPE) {
        recv_buf[len] = '\0';
        if(strcmp(recv_buf, "exit") == 0){
            return 1;
        }
        protocol_send(STR_TYPE,&recv_buf, len);;
    } 

    else if (type == FI32_TYPE) {
        protocol_send(FI32_TYPE,&recv_buf, len);
    } 

    else if (type == I16_TYPE) {
        protocol_send(I16_TYPE,&recv_buf, len);
    } 

    else if (type == UI16_TYPE) {
        protocol_send(UI16_TYPE,&recv_buf, len);
    } 

    else if (type == I32_TYPE) {
        protocol_send(I32_TYPE,&recv_buf, len);
    } 

    else if (type == UI32_TYPE) {
        protocol_send(UI32_TYPE,&recv_buf, len);
    }

    else {
        protocol_send(UI8_TYPE,&recv_buf, len);
    }
    
    return 0;
}

void check_send(void){
    uint8_t sample_uint8  = 42;
    int16_t sample_int16  = -1000;
    uint16_t sample_uint16 = 65530;
    int32_t sample_int32  = -1000000;
    uint32_t sample_uint32 = 4294967290U;
    float sample_float  = 3.14159f;
    char sample_string[] = "Hello PC!";

    protocol_send(UI8_TYPE,&sample_uint8, sizeof(sample_uint8));
    sleep_ms(200);

    protocol_send(I16_TYPE,&sample_int16, sizeof(sample_int16));
    sleep_ms(200);

    protocol_send(UI16_TYPE,&sample_uint16, sizeof(sample_uint16));
    sleep_ms(200);

    protocol_send(I32_TYPE,&sample_int32, sizeof(sample_int32));
    sleep_ms(200);

    protocol_send(UI32_TYPE,&sample_uint32, sizeof(sample_uint32));
    sleep_ms(200);

    protocol_send(FI32_TYPE,&sample_float, sizeof(sample_float));
    sleep_ms(200);

    protocol_send(STR_TYPE,&sample_string, sizeof(sample_string));
    sleep_ms(200);
}
