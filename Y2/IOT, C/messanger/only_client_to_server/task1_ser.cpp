#include  <util.hpp>
#include  <iot/socket.hpp>
#include  <arpa/inet.h>
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>
#include  <iostream>

int  main() {
uwe::set_ipaddr("192.168.1.8");
const  int server_port =  8888;

uwe::socket sock{AF_INET, SOCK_DGRAM, 0};
struct sockaddr_in server_address;
memset(&server_address, 0, sizeof(server_address));
server_address.sin_family  = AF_INET;
inet_pton(AF_INET, uwe::get_ipaddr().c_str(), &server_address.sin_addr);
server_address.sin_port  =  htons(server_port);

sock.bind((struct sockaddr *) &server_address, sizeof(server_address));

char* buffer = (char*)malloc(1024);
struct sockaddr client_address;
size_t client_address_size = sizeof(client_address);
memset(&client_address, 0, sizeof(client_address));

//recieve loop
while(sock.recvfrom(buffer, 1024,0, (struct sockaddr *)&client_address,&client_address_size)){

    std::cout<<buffer<<std::endl;

    //echo to the client
    sock.sendto(buffer, 1024,0, &client_address, client_address_size);
}

free(buffer);

return 0;
}