#include <stdint.h>
#include <stdio.h>

#define UI8_TYPE 0x01
#define I16_TYPE 0x02
#define UI16_TYPE 0x03
#define I32_TYPE 0x04
#define UI32_TYPE 0x05
#define FI32_TYPE 0x06
#define STR_TYPE 0x07

#define MAX_PAYLOAD_SIZE 64

void protocol_init(void);
int protocol_connect(const char* address, int port);
int protocol_send(uint8_t type, const void* data, int dataSize);
int protocol_receive(uint8_t* outType, void* buffer, int bufferSize);
void protocol_disconnect(int handle);
void protocol_cleanup(void);

int check_recieve(void);
void check_send(void);
