#pragma once
class TCPServer
{
public:
    TCPServer();
    ~TCPServer();
    void startUp();
    void acceptClients();
    int sendMessage(int sd, char* msg);
    void getTemp();
};

