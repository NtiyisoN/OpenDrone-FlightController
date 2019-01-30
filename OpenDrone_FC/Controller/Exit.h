#pragma once
#include "../Network/TCPServer.h"
#include <iostream>
class Exit
{
public:
	static Exit *getInstance();
	static Exit *instance;
	void sendError(int errorcode, bool stopFC);

private:
	Exit();
	~Exit();

	TCPServer *server;
};

