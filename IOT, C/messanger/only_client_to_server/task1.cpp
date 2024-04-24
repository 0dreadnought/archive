#include  <util.hpp>
#include  <iot/socket.hpp>
#include  <arpa/inet.h>
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>
#include  <iostream>

int  main() {

uwe::set_ipaddr("192.168.1.7");

const  int server_port =  8888;
const  char* server_name =  "192.168.1.8";

sockaddr_in server_address;
memset(&server_address, 0, sizeof(server_address));
server_address.sin_family  = AF_INET;
inet_pton(AF_INET, server_name, &server_address.sin_addr);
server_address.sin_port  =  htons(server_port);

uwe::socket sock{AF_INET, SOCK_DGRAM, 0};

const  int client_port =  1001;

struct  sockaddr_in  client_address;
memset(&client_address, 0, sizeof(client_address));
client_address.sin_family  = AF_INET;
client_address.sin_port  =  htons(client_port);
inet_pton(AF_INET, uwe::get_ipaddr().c_str(), &client_address.sin_addr);
sock.bind((struct  sockaddr  *)&client_address, sizeof(client_address));

std::string message =  "This is an IOT packet";

int len =  sock.sendto(
message.c_str(), message.length(), 0,
(sockaddr*)&server_address, sizeof(server_address));


//recieve the echo from the server

char* buffer = (char*)malloc(1024);
size_t server_address_size = sizeof(server_address);

sock.recvfrom(buffer, 1024,0, (sockaddr *)&server_address,&server_address_size);

std::cout<<buffer<<std::endl;

free(buffer);

return 0;
}