#pragma once
/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Markus Kurzmann
 * 	@version 0.0.1 07.01.2019
 */

#include <sys/time.h> 

class TCPServer
{
private:
	int opt = 1;
	int master_socket, addrlen, new_socket, activity, i, valread, sd;
	int max_sd;

	TCPServer();
	char buffer[1025];  //data buffer of 1K 
	bool isRunning = false;
	char* Temp;
	char *message = "Hello Client...";
	void addClient(int new_sock);
	void checkIOOperation(fd_set readfds);
public:
	bool connected = false;
	static TCPServer *getInstance();
	static TCPServer *instance;
	~TCPServer();
	void startUp();
	void acceptClients();
	int sendMessage(char* msg);
	void getTemp();
	void stopServer();
	void startSendingValues();

	int client_socket[30], max_clients = 30;
};

