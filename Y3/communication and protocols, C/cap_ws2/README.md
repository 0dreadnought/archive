# CaP_WS2

Custom communication protocol fr Rpi pico 

## Design

Raw byte transmission through USB using Rpi's putchar_raw(), recieve with get_char(). No encoding beyond utf-8, to keep as light as possible.
Corruption check using START byte, END byte, TYPE byte and CHECKSUM byte. Although,does not re-send corrupted or not recieved packets.

Connection established using Rpi's stdio_init_all() and stdio_usb_connected() and PySerial's Serial. No manual control over available connections on C side.

#### Packet structure:
| Start byte | Type byte | Datalen byte | Data byte * Datalen | Checksum byte | End byte |

- Start byte : Indicates begining of a packet = 0xFE
- Type byte : Indicates datatype of a trasmitted data = 0x01-...-0x07
- Datalen byte : Indicates length of a payload in bytes
- Data : data bytes || payload
- Checksum byte : error check mechanism. Calculated by 0 XOR each data byte.
- End byte : Indicates end of a packet = 0xFF

## Implementation

### C side:

No manual control over connections, so protocol_connect() and protocol_disconnect() left blank.
Also connection handles are not used.

protocol_init():

```c
stdio_init_all();

while (!stdio_usb_connected()) {
    sleep_ms(100);
}
```

For send, instead of handle, a data type indicator must be supplied.

```c
int protocol_send(uint8_t type, const void* data, int dataSize){
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
```

for recieve, instead of connection hadle, a datatype buffer must be supplied, which will be set to respective indicator.

```c
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

```


TYPE table is defined in header file:
```c
#define UI8_TYPE 0x01
#define I16_TYPE 0x02
#define UI16_TYPE 0x03
#define I32_TYPE 0x04
#define UI32_TYPE 0x05
#define FI32_TYPE 0x06
#define STR_TYPE 0x07
```

### Pyhton side

Proper encoding and decoding of datatypes longer than a byte is done using struct library.

Connection can be manually chosen over from a list of availible connections.
Only 1 connection at a time.
Avavailble ports are detected using PySerial's list_ports.comports().
```python
def try_connect(self):
    while(not self.connected):
        names = []
        ports = serial.tools.list_ports.comports()
        for port, desc, hwid in sorted(ports):
            names.append( port)
            print(f"{port}: {desc} [{hwid}]")
        x = input("Select port ")
        if x in names:
            self.connect(x)
```

To send a packet, only send data is required

```python
def send(self, data):
    datatype = get_datatype(data)
    if not self.connected:
        print("No connection")
        return
    fmt = TYPE_FORMATS.get(datatype)
    if not fmt:
        raise ValueError("Unsupported datatype")
    if datatype == 0x07:
        payload = data.encode('utf-8')
    else:
        payload = struct.pack(fmt, data)

    packet = bytearray()
   ... populate the packet...

    self.ser.write(packet)
```

recieve() will return decoded payload content. Flag can be set to false, to wait indefinately until anything recieved.

```python
def receive(self, flag = True):
    if not self.connected:
        print("No connection")
        return None
    
    ... wait until start byte ...

    type_byte = self.ser.read(1)[0]
    length = self.ser.read(1)[0]
    payload = self.ser.read(length)
    checksum = self.ser.read(1)[0]
    end = self.ser.read(1)[0]

    ... check if checksum and end byte are good ...

    fmt = TYPE_FORMATS.get(type_byte)

    if not fmt:
        print(f"Unsupported data type: {type_byte}")
        return None
    
    if type_byte == 0x07:
        return payload.decode('utf-8')

    print(f"Type: {type_byte}, Format: {fmt}, Length: {length}, Payload: {payload.hex()}")
    
    return list(struct.unpack(fmt, payload))[0]
```

TYPE table:
```python
TYPE_FORMATS = {
    0x01: 'B',
    0x02: 'h',
    0x03: 'H',
    0x04: 'i',
    0x05: 'I',
    0x06: 'f',
    0x07: 's',
}
```

## Testing

After connecting to the machine, "echo" or "listen" commands can be sent to machine to test the protocol.
For "listen" command, machine will send a bunch of test data of different types.
For "echo" command, test data of different types will be sent to machine, which it will receive, decode, encode and send back.
It will continue to echo everything sent, until "exit" command is sent.
