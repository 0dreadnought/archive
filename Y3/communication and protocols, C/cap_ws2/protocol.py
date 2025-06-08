import serial
import serial.tools.list_ports
import struct

from time import sleep

START_BYTE = 0xFE
END_BYTE = 0xFF
BAUD_RATE = 9600

TYPE_FORMATS = {
    0x01: 'B',
    0x02: 'h',
    0x03: 'H',
    0x04: 'i',
    0x05: 'I',
    0x06: 'f',
    0x07: 's',
}

def get_datatype(data):
    if isinstance(data, int):
        if 0 <= data <= 0xFF:
            return 0x01 
        elif -32768 <= data <= 32767:
            return  0x02
        elif 0 <= data <= 0xFFFF:
            return  0x03
        elif -2147483648 <= data <= 2147483647:
            return 0x04
        elif 0 <= data <= 0xFFFFFFFF:
            return 0x05
        else:
            raise ValueError("Integer too large")

    elif isinstance(data, float):
        return 0x06  

    elif isinstance(data, str):
        return 0x07  
    

    else:
        raise TypeError("Unsupported data type")

class Protocol:
    def __init__(self):
        self.connected = False
        self.ser = 0

    def connect(self, port):
        if not self.connected:
            self.ser = serial.Serial(port = port, baudrate = BAUD_RATE, timeout=1)
            #self.ser.open()
            self.connected = True
            return
        print("alredy connected")

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
        packet.append(START_BYTE)
        packet.append(datatype)
        packet.append(len(payload))
        checksum = 0

        for b in payload:
            packet.append(b)
            checksum ^= b

        packet.append(checksum)
        packet.append(END_BYTE)

        self.ser.write(packet)

    def receive(self, flag = True):
        if not self.connected:
            print("No connection")
            return None
        
        c = 0
        
        while True:
            x = self.ser.read(1)
            if len(x) == 0:
                continue
            if x[0] == START_BYTE:
                break
            c+=1
            if c > 300 and flag:
                return None
        type_byte = self.ser.read(1)[0]
        length = self.ser.read(1)[0]
        payload = self.ser.read(length)
        checksum = self.ser.read(1)[0]
        end = self.ser.read(1)[0]

        calc_checksum = 0
        for b in payload:
            calc_checksum ^= b

        if end != END_BYTE or checksum != calc_checksum:
            print("Checksum or end byte error")
            return

        fmt = TYPE_FORMATS.get(type_byte)

        if not fmt:
            print(f"Unsupported data type: {type_byte}")
            return None
        
        if type_byte == 0x07:
            return payload.decode('utf-8')

        print(f"Type: {type_byte}, Format: {fmt}, Length: {length}, Payload: {payload.hex()}")
        
        return list(struct.unpack(fmt, payload))[0]

    def disconnect(self):
        if self.connected:
            self.ser.close()
            self.connected = False
            return
        print("Not connected anyway")        

    def cleanup(self):
        self.disconnect()

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

    def check_send(self):
        sent = 42
        self.send(sent)
        sleep(0.2)
        recieve = self.receive(False)
        print(str(sent) + "->" + str(recieve))
        sleep(0.2)

        sent = -1234
        self.send(sent)
        sleep(0.2)
        recieve = self.receive(False)
        print(str(sent) + "->" + str(recieve))
        sleep(0.2)

        sent = 3.14
        self.send(sent)
        sleep(0.2)
        recieve = self.receive(False)
        print(str(sent) + "->" + str(recieve))
        sleep(0.2)

        sent = "Hello Pico!"
        self.send(sent)
        sleep(0.2)
        recieve = self.receive(False)
        print(str(sent) + "->" + str(recieve))
        sleep(0.2)
    
    def check_recieve(self):
        for i in range(7):
            result = connection.receive(False)
            print(result)
            sleep(0.2)


if(__name__ =="__main__"):
    
    connection = Protocol()
    connection.try_connect()

    while True:
        x = input("cli: ")

        if x == "send":
            x = input("data:")
            connection.send(x)
            if x == "echo":
                sleep(0.2)
                result = connection.receive(False)
                print(result)
                connection.check_send()
            if x == "listen":
                result = connection.receive(False)
                print(result)
                connection.check_recieve()
        
        elif x == "get":
            result = connection.receive()
            print(result)
        
        elif x == "disc":
            connection.disconnect()

        elif x == "open":
            connection.try_connect()

        elif x == "exit":
            break

        else:
            print("Unkown command")
    

