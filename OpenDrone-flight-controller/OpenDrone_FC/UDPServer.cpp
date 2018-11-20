// Server side C/C++ program to demonstrate Socket programming 
#include "UDPServer.h"
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>
#define PORT 2018 
using namespace std;

UDPServer::UDPServer()
{
}

int UDPServer::test()
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address,
		sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}


	while (true)
	{
		buffer[1024] = { 0 };
		if (listen(server_fd, 3) < 0)
		{
			perror("listen");
			exit(EXIT_FAILURE);
		}

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
			(socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		valread = read(new_socket, buffer, 1024);
		cout << buffer;
		cout.flush();
		char *hello = "Hello from server";
		send(new_socket, hello, strlen(hello), 0);
	}
	return 0;
}

UDPServer::~UDPServer()
{
}