#pragma once
/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */

class TCPServer
{
public:
    TCPServer();
    ~TCPServer();
    void startUp();
    void acceptClients();
    int sendMessage(int sd, char* msg);
};

